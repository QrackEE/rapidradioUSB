using System;
using System.Diagnostics;

namespace rapidradioUSB_win_driver_installer
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine();
            Console.WriteLine("rapidradio USB driver installer");
            Console.WriteLine("Operating System type: {0}-bit", Environment.Is64BitOperatingSystem ? "64" : "32");
            Console.WriteLine("Installing INF file...");

            var dpinstPath = Environment.Is64BitOperatingSystem ? @"files\amd64\dpinst.exe" : @"files\x86\dpinst.exe";

            var dpinst = Process.Start(new ProcessStartInfo(dpinstPath, "/PATH .. /F")
            {
                UseShellExecute = false,
                RedirectStandardOutput = true
            });

            if (dpinst != null)
            {
                dpinst.WaitForExit();
                Console.Write(dpinst.StandardOutput.ReadToEnd());
                Console.WriteLine("Done!");
            }
            else
            {
                Console.WriteLine("ERROR: Cannot run the dpinst.exe process.");
            }
        }
    }
}
