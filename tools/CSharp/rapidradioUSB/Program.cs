using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using MonoLibUsb;
using MonoLibUsb.Profile;

namespace rapidradioUSB
{
    class Program
    {
        private const int Vid = 0x0000;
        private const int Pid = 0x0001;
        private static bool _stopRequested;
        private static IntPtr _unmanagedReadBuff;
        private static IntPtr _unmanagedWriteBuff;
        private static bool _stats;
        private static Thread _readingThread;
        private static readonly Regex RegisterRegex = new Regex("^-r([01])_([a-f0-9]{2})=0x([a-f0-9]{2,22})$", RegexOptions.Compiled | RegexOptions.IgnoreCase);

        private static readonly byte[] TransmissionEndToken = {0xb3, 0x67, 0x2b, 0x26, 0xa2, 0x33, 0x43, 0x25, 0x80, 0x50, 0x80, 0xd5, 0x6e, 0xad, 0x94, 0x85,
										                       0x0c, 0xec, 0xf8, 0x0f, 0x57, 0x8c, 0x48, 0xda, 0x95, 0x2d, 0x09, 0xbf, 0xc0, 0x04, 0xd5, 0x40};

        private enum RadioMode
        {
            Listening = 0,
            Sending = 1,
            Idle = 2
        }

        private class Params
        {
            internal class RfmRegister
            {
                public byte Bank { get; set; }
                public byte Register { get; set; }
                public List<byte> Value { get; set; }

                public byte[] Serialize()
                {
                    var result = new byte[16];

                    result[0] = 0xA2;

                    result[1] = Bank;
                    result[2] = Register;
                    Value.CopyTo(result, 3);
                    result[14] = (byte) Value.Count;

                    result[15] = 0xB3;

                    return result;
                }
            }

            public byte Channel { get; set; }
            public uint Address { get; set; }
            public bool Verbose { get; set; }
            public bool Ack { get; set; }
            public bool PacketNumbering { get; set; }
            public byte Retries { get; set; }
            public RadioMode Mode { get; set; }
            public List<RfmRegister> RfmRegisters { get; set; }
            public List<byte> SinglePacket { get; set; }
            public bool HexOutput { get; set; }
            public bool NewLineAfterPacket { get; set; }

            public Params()
            {
                RfmRegisters = new List<RfmRegister>();
            }

            public IEnumerable<byte[]> SerializeRfmRegisters()
            {
                if (RfmRegisters == null) yield break;

                foreach (var reg in RfmRegisters)
                {
                    yield return reg.Serialize();
                }
            }

            // only important settings are being sent to the USB device
            public byte[] Serialize()
            {
                var result = new byte[11];

                // start token
                result[0] = 0xA1;

                result[1] = Channel;

                Func<IEnumerable<byte>, IEnumerable<byte>> transformByteOrder = b => b;
                if (!BitConverter.IsLittleEndian)
                {
                    transformByteOrder = b => b.Reverse();
                }

                // Address
                int i = 2;
                foreach (var b in transformByteOrder(BitConverter.GetBytes(Address)))
                {
                    result[i++] = b;
                }

                result[6] = Ack ? (byte)1 : (byte)0;

                result[7] = Verbose ? (byte) 1 : (byte) 0;

                result[8] = Retries;

                result[9] = (byte) Mode;

                // end token
                result[10] = 0xB2;

                return result;
            }
        }

        private static void Usage()
        {
            Console.WriteLine();
            Console.WriteLine("This is a command-line tool to control the rapidradio USB module. Please visit http://rapidradio.pl/ for more informations.");
            Console.WriteLine();
            Console.WriteLine("Usage:");
            Console.WriteLine("rapidradioUSB.exe [-d=<device_number>] [-l] [-c=<channel>] [-a=<hex_address>] [-v] [-nack] [-i] [-t=<retries>] [-rN=<single_byte_value>] [-p=<single_hex_packet>]");
            Console.WriteLine();
            Console.WriteLine("-d=<device_number>      Device number (0-based). Use that option if more than one rapidradio USB is attached.");
            Console.WriteLine("-l                      Listening Mode (receiving data). Default mode is Sending Mode (transmitting data).");
            Console.WriteLine("-c=<channel>            Radio channel number, a value from range: 1 to 83.");
            Console.WriteLine("-a=<hex_address>        It's a 4-byte hex address used for listening or target device address when sending.");
            Console.WriteLine("-v                      Verbose output.");
            Console.WriteLine("-nack                   Don't expect ACKs when sending. Speeds up transmission but gives no guarantee that all packets reach the target device.");
            Console.WriteLine("-i                      Ignore packet number byte. Use for real raw transmissions.");
            Console.WriteLine("-t=<retries>            Amount of retry attempts when a packet has not been sent successfully. A value within range 0-255.");
            Console.WriteLine("-p=<single_hex_packet>  Sends just one single packet. It must contain 1-32 bytes (hex formatted, 2 characters per each byte). Example: -p=AA1100FF will send 4-bytes long packet.");
            Console.WriteLine("-r<bank>_<hex_reg>=0x<hex_value>\n\tManual setting for particular RFM's register. E.g. usage: to change auto-retry behavior to just 2 retries and 4ms interval use:");
            Console.WriteLine("\t-r0_04=0xF2 which means: put F2 (hex) value into the RFM's register no. 4 (hex) in bank 0.");
            Console.WriteLine("\t<bank>: 0 or 1 only");
            Console.WriteLine("\t<hex_reg>: One-byte hex formatted register number. Always 2-digits (zero-prefixed), e.g.: 00 or 01 or 1A");
            Console.WriteLine("\t<hex_value>: 1-11 bytes hex formatted register value. Always even number of digits (zero-prefixed).");
            Console.WriteLine();
            Console.WriteLine("Example:");
            Console.WriteLine("rapidradioUSB.exe -c=10 -a=0xABCD0102 -v -nack -t=5 < file_to_send.txt");
            Console.WriteLine();
            Console.WriteLine("Example:");
            Console.WriteLine("rapidradioUSB.exe -c=10 -a=0xABCD0102 -v -r0_04=0xF2 -p=00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF");
            Console.WriteLine();
            Console.WriteLine("Example:");
            Console.WriteLine("rapidradioUSB.exe -l -c=10 -a=0xABCD0102 -v > received_file.txt");
            Console.WriteLine();
        }

        private static string GetErrorMessage(int e)
        {
            return MonoUsbApi.StrError((MonoUsbError) e) + "; Last error string: " + MonoUsbSessionHandle.LastErrorString;
        }

        static void Main(string[] args)
        {
            MonoUsbSessionHandle sessionHandle = null;
            MonoUsbDeviceHandle rapidradio = null;
            try
            {
                if (args.Length == 1 && args[0] == "--help")
                {
                    Usage();
                    return;
                }

                var cmdParams = ParseCmdParams(args);

                // set up USB lib
                sessionHandle = new MonoUsbSessionHandle();
                if (sessionHandle.IsInvalid) throw new Exception("Invalid session handle.");

                MonoUsbProfileListHandle list;
                MonoUsbApi.GetDeviceList(sessionHandle, out list);

                var profileList = new MonoUsbProfileList();
                profileList.Refresh(sessionHandle);
                var rapidradios = profileList.Where(d => d.DeviceDescriptor != null && d.DeviceDescriptor.VendorID == Vid && d.DeviceDescriptor.ProductID == Pid).ToArray();

                if (rapidradios.Length == 0)
                {
                    Error("rapidradio USB device not found");
                }

                if (rapidradios.Length > 1)
                {
                    // more than 1 rapidradio USB attached
                    Error(string.Format("Detected {0} rapidradio USB modules - currently only one device is supported.", rapidradios.Length), false);
                }

                rapidradio = rapidradios[0].OpenDeviceHandle();
                if (rapidradio == null || rapidradio.IsInvalid) throw new Exception("Invalid session handle.");
                var r = MonoUsbApi.SetConfiguration(rapidradio, 1);
                if (r != 0) Error("SetConfiguration error: " + GetErrorMessage(r));
                r = MonoUsbApi.ClaimInterface(rapidradio, 0);
                if (r != 0) Error("ClaimInterface error: " + GetErrorMessage(r));

                _unmanagedReadBuff = Marshal.AllocHGlobal(32);
                _unmanagedWriteBuff = Marshal.AllocHGlobal(32);

                // send configuration to endpoint 4
                var settings = cmdParams.Serialize();
                SendData(rapidradio, 4, settings, settings.Length, 200);

                // send custom register settings (if any)
                foreach (var regPacket in cmdParams.SerializeRfmRegisters())
                {
                    if (cmdParams.Verbose)
                    {
                        Info(string.Format("Setting register R{0}={1}", regPacket[1], regPacket[2]));
                    }

                    SendData(rapidradio, 4, regPacket, regPacket.Length);
                }

                var stopWatch = new Stopwatch();
                if (_stats) stopWatch.Start();

                if (cmdParams.Verbose)
                {
                    Info(string.Format("Address = 0x{0}", cmdParams.Address.ToString("X8")));
                    Info(string.Format("Channel = {0}", cmdParams.Channel));
                    Info(string.Format("Mode = {0}", cmdParams.SinglePacket == null ? cmdParams.Mode.ToString() : "Single Packet Mode"));
                    Info(string.Format("ACK {0}", cmdParams.Ack ? "enabled" : "disabled"));
                    Info(string.Format("Retries = {0}", cmdParams.Retries));
                    if (cmdParams.SinglePacket == null)
                    {
                        Info(string.Format("Packet Numbering {0}", cmdParams.PacketNumbering ? "enabled" : "disabled"));
                    }
                }

                if (cmdParams.SinglePacket != null)
                {
                    // send exactly one packet
                    StartReadingThread(rapidradio);
                    SendData(rapidradio, 2, cmdParams.SinglePacket.ToArray(), cmdParams.SinglePacket.Count);
                    StopReadingThread();
                }
                else
                {
                    switch (cmdParams.Mode)
                    {
                        case RadioMode.Listening:
                            Listening(rapidradio, cmdParams);
                            break;
                        case RadioMode.Sending:
                            Sending(rapidradio, cmdParams);
                            break;
                        default:
                            throw new ArgumentOutOfRangeException("Unknown mode");
                    }
                }

                if (_stats) Info("Transmission took " + stopWatch.Elapsed);

                // switch off the radio
                TurnOff(rapidradio);
            }
            catch (Exception e)
            {
                Error("An error occured: " + GetRecursiveMessage(e, 10));
            }
            finally
            {
                {
                    // Free and close resources
                    if (rapidradio != null)
                    {
                        if (!rapidradio.IsInvalid)
                        {
                            MonoUsbApi.ReleaseInterface(rapidradio, 0);
                            rapidradio.Close();
                        }
                    }

                    if (sessionHandle != null)
                    {
                        sessionHandle.Close();
                    }

                    Marshal.FreeHGlobal(_unmanagedReadBuff);
                    Marshal.FreeHGlobal(_unmanagedWriteBuff);
                }
            }
        }

        private static void TurnOff(MonoUsbDeviceHandle rapidradio)
        {
            var cmdParams = new Params {Mode = RadioMode.Idle};
            var settings = cmdParams.Serialize();
            SendData(rapidradio, 4, settings, settings.Length, 1000);
        }

        private static void ReadFromUsbAndFlushToOutput(object context)
        {
            try
            {
                using (var stream = Console.OpenStandardError())
                {
                    var buff = new byte[32];
                    var rapidradio = (MonoUsbDeviceHandle) context;
                    while (!_stopRequested)
                    {
                        var read = ReadData(rapidradio, 3, buff, 32, 50);
                        if (read > 0)
                        {
                            stream.Write(buff, 0, read);
                            stream.Flush();
                            if (Encoding.ASCII.GetString(buff).ToLowerInvariant().Contains("unable to send"))
                            {
                                _stopRequested = true;
                                TurnOff(rapidradio);
                                Error("Exiting because rapidradioUSB is unable to send a packet");
                            }
                        }

                        if (read == 0)
                        {
                            Thread.Sleep(100);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                if (!_stopRequested)
                {
                    Error(GetRecursiveMessage(e, 10), false);
                }
            }
        }

        private static void StartReadingThread(MonoUsbDeviceHandle rapidradio)
        {
            _readingThread = new Thread(ReadFromUsbAndFlushToOutput);
            _readingThread.Start(rapidradio);
        }

        private static void StopReadingThread()
        {
            _stopRequested = true;
            if (!_readingThread.Join(500))
            {
                Error("Reading thread didn't stop yet.", false);
            }
        }

        private static void Sending(MonoUsbDeviceHandle rapidradio, Params cmdParams)
        {
            StartReadingThread(rapidradio);

            try
            {
                using (var stream = Console.OpenStandardInput())
                {
                    byte packetNumber = 0;
                    var buff = new byte[32];
                    while (true)
                    {
                        int read = 0;
                        int b;
                        int offset = cmdParams.PacketNumbering ? 1 : 0;
                        do
                        {
                            b = stream.ReadByte();
                            if (b > -1)
                            {
                                buff[offset + read++] = (byte)b;
                            }
                        } while (read < 32 - offset && b != -1 && b != '\r' && b != '\n');

                        if (read > 0)
                        {
                            try
                            {
                                if (cmdParams.PacketNumbering)
                                {
                                    buff[0] = packetNumber++;
                                }

                                var buffCopy = new byte[32];
                                Array.Copy(buff, buffCopy, read + (cmdParams.PacketNumbering ? 1 : 0));
                                SendData(rapidradio, 2, buffCopy, read + (cmdParams.PacketNumbering ? 1 : 0));
                            }
                            catch (Exception e)
                            {
                                Error(e.Message, false);
                            }
                        }

                        if (b == -1) break;
                    }
                }

                // send transmission end packet
                try
                {
                    SendData(rapidradio, 2, TransmissionEndToken, 32);
                }
                catch (Exception e)
                {
                    Error(e.Message, false);
                }
            }
            catch (Exception e)
            {
                Error(GetRecursiveMessage(e, 10));
            }

            StopReadingThread();
        }

        private static void Listening(MonoUsbDeviceHandle rapidradio, Params cmdParams)
        {
            using (var stream = Console.OpenStandardOutput())
            {
                var newline = Encoding.ASCII.GetBytes(Environment.NewLine);
                byte packetNumber = 0;
                var buff = new byte[32];
                while (true)
                {
                    var read = ReadData(rapidradio, 3, buff, 32, 50);
                    if (read > 0)
                    {
                        if (read == 32 && TransmissionEndToken.SequenceEqual(buff))
                        {
                            // got transmission end token
                            break;
                        }

                        bool outputData = false;
                        if (cmdParams.PacketNumbering)
                        {
                            if (buff[0] > packetNumber)
                            {
                                if (cmdParams.Verbose)
                                {
                                    Error(string.Format("Lost packets, expected packet {0} but got {1}", packetNumber, buff[0]), false);
                                }

                                packetNumber = buff[0];
                                outputData = true;
                            }
                            else if (buff[0] < packetNumber)
                            {
                                if (cmdParams.Verbose)
                                {
                                    Error(string.Format("Duplicated packet, expected packet {0} but got {1}", packetNumber, buff[0]), false);
                                }
                            }
                            else
                            {
                                outputData = true;
                            }
                        }
                        else
                        {
                            outputData = true;
                        }

                        if (outputData)
                        {
                            packetNumber++;
                            var offset = cmdParams.PacketNumbering ? 1 : 0;

                            if (!cmdParams.HexOutput)
                            {
                                stream.Write(buff, offset, read - offset);
                            }
                            else
                            {
                                // .TaArray() added due to Mono compatibility issues
                                var hexOutput = Encoding.ASCII.GetBytes(string.Join(" ", buff.Skip(offset).Select(b => b.ToString("X2")).ToArray()));
                                stream.Write(hexOutput, 0, hexOutput.Length);
                            }

                            if (cmdParams.NewLineAfterPacket)
                            {
                                stream.Write(newline, 0, newline.Length);
                            }

                            stream.Flush();
                        }
                    }
                }
            }
        }

        private static void SendData(MonoUsbDeviceHandle rapidradio, int endpoint, byte[] data, int length, int timeout = 1000000)
        {
            Marshal.Copy(data, 0, _unmanagedWriteBuff, Math.Min(length, 32));

            int transferred;
            var r = MonoUsbApi.BulkTransfer(rapidradio,
                (byte) endpoint,
                _unmanagedWriteBuff,
                length,
                out transferred,
                timeout);

            if (r != (int) MonoUsbError.Success)
            {
                throw new Exception("Error while sending: " + GetErrorMessage(r));
            }
        }

        private static int ReadData(MonoUsbDeviceHandle rapidradio, int endpoint, byte[] data, int length, int timeout = 1)
        {
            int transferred;
            var r = MonoUsbApi.BulkTransfer(rapidradio,
                (byte) (endpoint + 0x80),
                _unmanagedReadBuff,
                length,
                out transferred,
                timeout);

            if (r != (int) MonoUsbError.Success && r != (int) MonoUsbError.ErrorTimeout)
            {
                throw new Exception("Error while reading: " + GetErrorMessage(r));
            }

            Marshal.Copy(_unmanagedReadBuff, data, 0, Math.Min(transferred, 32));

            return transferred;
        }

        private static void Error(string msg, bool exit = true)
        {
            Console.Error.WriteLine("ERROR: " + msg);

            if (exit)
            {
                Environment.Exit(1);
            }
        }

        private static void Info(string msg)
        {
            Console.Error.WriteLine(msg);
        }

        private static Params ParseCmdParams(string[] args)
        {
            var cmdParams = new Params
            {
                Channel = 55,
                Address = 0xAABB0001,
                PacketNumbering = true,
                Ack = true,
                Retries = 10,
                Mode = RadioMode.Sending
            };

            foreach (var cmd in args)
            {
                if (cmd.StartsWith("-c="))
                {
                    byte channel;
                    if (!byte.TryParse(cmd.Substring(3), out channel) || channel < 1 || channel > 83)
                    {
                        Error("Invalid channel number (-c option)");
                    }

                    cmdParams.Channel = channel;
                }
                else if (cmd.StartsWith("-a="))
                {
                    var sadr = cmd.Substring(3);
                    if (sadr.StartsWith("0x"))
                    {
                        sadr = sadr.Substring(2);
                    }

                    uint address;
                    if (!uint.TryParse(sadr, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out address))
                    {
                        Error("Invalid Address (-a option)");
                    }

                    cmdParams.Address = address;
                }
                else if (cmd.StartsWith("-t="))
                {
                    byte retries;
                    if (!byte.TryParse(cmd.Substring(3), out retries))
                    {
                        Error("Invalid amount of retries (-t option)");
                    }

                    cmdParams.Retries = retries;
                }
                else if (cmd.StartsWith("-r"))
                {
                    var match = RegisterRegex.Match(cmd);
                    if (match.Success)
                    {
                        var rfmReg = new Params.RfmRegister {Value = new List<byte>()};
                        try
                        {
                            rfmReg.Bank = byte.Parse(match.Groups[1].Value, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                            rfmReg.Register = byte.Parse(match.Groups[2].Value, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                        }
                        catch (Exception)
                        {
                            Error(string.Format("Cannot parse hex number. Please check the {0} parameter.", cmd));
                        }

                        var sval = match.Groups[3].Value;
                        if (sval.ToLowerInvariant().StartsWith("0x"))
                        {
                            sval = sval.Substring(2);
                        }

                        if (sval.Length%2 == 1)
                        {
                            Error(string.Format("Register value '{0}' should contain hex formatted bytes, so has to have even number of characters (2 hex character per each byte).", sval));
                        }

                        for (int i = 0; i < sval.Length/2; i++)
                        {
                            byte b;
                            if (!byte.TryParse(sval.Substring(i*2, 2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out b))
                            {
                                Error(string.Format("Cannot parse '{0}' as a valid hex byte. Please check register value '{1}' in the parameter: '{2}'.", sval.Substring(i * 2, 2), sval, cmd));
                            }

                            rfmReg.Value.Add(b);
                        }

                        cmdParams.RfmRegisters.Add(rfmReg);
                    }
                    else
                    {
                        Error("Invalid format of -r option. Example of valid RFM register setting: -r0_04=0xF2. Bank number can be 0 or 1 only and both reg number and reg value should be hex formatted.");
                    }
                }
                else if (cmd.StartsWith("-p="))
                {
                    var sbytes = cmd.Substring(3);
                    if (sbytes.ToLowerInvariant().StartsWith("0x"))
                    {
                        sbytes = sbytes.Substring(2);
                    }

                    if (sbytes.Length%2 == 1 || sbytes.Length == 0 || sbytes.Length > 64)
                    {
                        Error("Invalid length of packet (-p option). It has to be 1-32 bytes (so 2-64 hex characters, always even amount of characters).");
                    }

                    cmdParams.SinglePacket = new List<byte>(sbytes.Length/2);
                    for (int i = 0; i < sbytes.Length/2; i++)
                    {
                        byte b;
                        if (!byte.TryParse(sbytes.Substring(i*2, 2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out b))
                        {
                            Error(string.Format("Can't parse {0} as byte. The packet has to be hex formatted, so 0-9 digits and A-F characters are allowed only.", sbytes.Substring(i*2, 2)));
                        }

                        cmdParams.SinglePacket.Add(b);
                    }
                }
                else switch (cmd)
                {
                    case "-v":
                        cmdParams.Verbose = true;
                        break;
                    case "-nack":
                        cmdParams.Ack = false;
                        break;
                    case "-i":
                        cmdParams.PacketNumbering = false;
                        break;
                    case "-l":
                        cmdParams.Mode = RadioMode.Listening;
                        break;
                    case "-stats":
                        _stats = true;
                        break;
                    case "-x":
                        cmdParams.HexOutput = true;
                        break;
                    case "-n":
                        cmdParams.NewLineAfterPacket = true;
                        break;
                    default:
                        Error("Unknown command-line argument: " + cmd);
                        break;
                }
            }

            return cmdParams;
        }

        private static string GetRecursiveMessage(Exception e, int maxDepth)
        {
            var exceptionMessage = e.Message;

            var currentDepth = 1;
            while (currentDepth++ <= maxDepth && e.InnerException != null)
            {
                exceptionMessage += string.Format(", {0}", e.InnerException.Message);
                e = e.InnerException;
            }
            return exceptionMessage;
        }
    }
}
