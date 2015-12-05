//constant definitions, prototype function definitions

#ifndef Includes
#define Includes

//constant definitions
//estados LED 
#define ledRed        1
#define ledGreen	  2
#define ledOrange     3
#define ledOff        0








//DEFINICION DE COMANDOS PARA COM
#define intComandoComDispositivo        1 
#define intComandoMedirTempAmbiente     2 
#define intComandoDiagnosticarEquipo    3 
#define intComandoDiagnosticoProgreso   4 
#define intComandoDiagnosticoCancelado  5 
#define intComandoDiagnosticoFinalizado 6
#define intComandoDesconectarEquipo     7
#define intComandoCalibracion           8
#define intComandoCalibracionMedicion   9
#define intComandoRectificador          10
#define intComandoRectificadorMedicion  11
#define intComandoRectificadorFinal     12
#define intComandoDiodo                 13
#define intComandoDiodoTrazar           14
#define intComandoDiodoTrazarValor      15
#define intComandoDiodoFinal            16
#define intComandoZener                 17
#define intComandoZenerMedicion         18
#define intComandoZenerFinal            19
#define intComandoLed                   20
#define intComandoLedMedicion           21
#define intComandoLedFinal              22
#define intComandoPuente                23
#define intComandoPuenteMedicion        24
#define intComandoPuenteFinalVf         25
#define intComandoPuenteFinalIr         26
#define intComandoPuenteFinalTa         27
#define intComandoIdDiodo               28
#define intComandoIdDiodoMedicion       29
#define intComandoIdDiodoFinal          30



//#define intTest     25
 

//Datos com  
#define datoDispositivoNinguno     '0'
#define datoDispositivoDiodo       '1'
#define datoDispositivoTransistor  '2'
#define datoDispositivoSCR         '3'
#define datoDispositivoTriac       '4'
#define datoDispositivoDiac        '5'

//unidades y múltiplos
#define micro     'u'
#define mili      'm'
#define Amper     'A'
#define Empty     ' '
#define Voltio    'V'

//--------------------------------------------------------------------
// prototype function definitions
void Delay_1s(void);
/*void Delay_100ms(void);
void Delay_500ms(void);
void Delay_1s(void);
void BuzzerOk(void);
void BuzzerError(void);

dword MeasureAdc (byte adcSelection);
void ProcesaRxUSB (void);
void ExtraProcess (void);
void RxSciProcess (void);
byte ConvierteStringToNumber (byte ValorAlto, byte ValorBajo);
byte ConvierteNumberToStringAlto (byte valor);
byte ConvierteNumberToStringBajo (byte valor);
word CalcularCorriente (word valorResistencia, dword valorTension);
byte GetDigitFromNumber (word number, byte digitPointer);

void GetCurrentSupplySpiData(byte currentUnit, byte currentHundredFigure, byte currentTenFigure, byte currentUnitFigure);
void GetCalibrationCurrentSupplySpiData(byte typeControl);
void GetSensorCalibrationCurrentSupplySpiData(byte typeControl);

void GetVoltageSupplySpiData(byte voltageUnit, byte voltageHundredFigure, byte voltageTenFigure, byte voltageUnitFigure);
//void GetCalibrationCurrentSupplySpiData(byte typeControl);
//void GetSensorCalibrationCurrentSupplySpiData(byte typeControl);

void ClearSpiData(void);

void SendBlockDataFuentesI(byte dataFuenteI0, byte dataFuenteI1, byte dataFuenteI2, byte dataFuenteI3, byte dataFuenteI4);
void SendDataSpiFuentesI(byte dataFuenteI);
void SetResetFuentesI(void);
void SetNoResetFuentesI(void);
void SetOutputEnableFuentesI(void);
void SetOutputHiImpedanceFuentesI(void);
void SetLatchClockFuentesI(void);
void SendInitFuentesI(void);

void SendBlockDataFuentesV(byte dataFuenteV0, byte dataFuenteV1);
void SendDataSpiFuentesV(byte dataFuenteV);
void SetResetFuentesV(void);
void SetNoResetFuentesV(void);
void SetOutputEnableFuentesV(void);
void SetOutputHiImpedanceFuentesV(void);
void SetLatchClockFuentesV(void);
void SendInitFuentesV(void);

void DiagnoseFuentes(void);

word MeasureSupplies(byte supplyType, byte supplyNumberHigh, byte supplyNumberLow);


dword CompensaPorResistenciasParalelo (dword corrienteTensionReal); //VER SI SE USA


//void SendUSB (byte highCommand, byte lowCommand, byte sizeBufferData, byte bufferData[]);
void SendUSB (byte highCommand, byte lowCommand, byte sizeBufferData, 
              byte bData0,
              byte bData1,
              byte bData2,
              byte bData3,
              byte bData4,
              byte bData5,
              byte bData6,
              byte bData7,
              byte bData8,
              byte bData9,
              byte bData10,
              byte bData11,
              byte bData12,
              byte bData13,
              byte bData14,
              byte bData15,
              byte bData16,
              byte bData17,
              byte bData18,
              byte bData19);
              
//void SendSCI (byte highCommand, byte lowCommand, byte sizeBufferData, byte bufferData[]);
void SendSCI (byte highCommand, byte lowCommand, byte sizeBufferData, 
              byte bData0,
              byte bData1,
              byte bData2,
              byte bData3,
              byte bData4,
              byte bData5,
              byte bData6,
              byte bData7,
              byte bData8,
              byte bData9,
              byte bData10,
              byte bData11,
              byte bData12,
              byte bData13,
              byte bData14,
              byte bData15,
              byte bData16,
              byte bData17,
              byte bData18,
              byte bData19);
              
//---------------------------------------------------                            
#define Wait()  { asm wait; }      //wait

//---------------------------------------------------                            
*/

#endif
