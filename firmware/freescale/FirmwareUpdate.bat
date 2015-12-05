
IF NOT EXIST "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16" (
	echo USBDM Not found
	pause
	exit
)

IF NOT EXIST "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\bak" (
	md "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\bak"
	xcopy /Y "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\bak\*.sx" "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\bak\"
	del /Q /F "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\bak\*.sx"
)

xcopy /Y "bin\Project.abs.s19" "C:\Program Files (x86)\pgo\USBDM 4.10.6.250\FlashImages\JS16\USBDM_JS16CWJ_V4.sx"
"C:\Program Files (x86)\pgo\USBDM 4.10.6.250\JS16_Bootloader.exe"

