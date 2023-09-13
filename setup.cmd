@ECHO OFF
ECHO Using SSD (C:) for VMs

MD C:\Users\stich\Documents\unir\tfm\lab-setup-2\vbox
"C:\Program Files\Oracle\VirtualBox\VBoxManage.exe" setproperty machinefolder "C:\Users\stich\Documents\unir\tfm\lab-setup-2\vbox"