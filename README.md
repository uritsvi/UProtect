#UProtect

A simple kernel driver project that can protect files and folders, 
have support for anty tampering and uses the 
aho corasick algorithm to protect file paths.

<Img src="UProtectExample.gif"/>

<h2>Build Requirements</h2>
- [Visual studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false)
- [WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) 

<h2>Build</h2>
- Download the code and open the ```.sln``` file in visual studio
- Press ```shift+b``` to builed the code 

<h2>RUN</h2>
- Install the src code builed the code with the instration above and move the ``bin`` file to ```C:\UPROTECT``` 
- Run ```sc create Kprotect type=kernel binPath=C:\UPROTECT\bin\KProtector``` 
- Open ```RegEdit``` on youre pc 
- Create A new key under ```HKEY_LOCAL_MACHINE/SOFTWARE``` and name it ```UProtect```
- Under that key create a sub key named ```MINIFILTER```, And under it create another sub key named ```Build``` 
- Open the ``bin`` file and run the executble named ```UProtectApp.exe```
- (Recommended) VM
- The hard coded password is ```uri```

<h2>Future Goals</h2>
- Implement an installer
- Implement a registry protector window
- Store the password in the registry and make the installer set it

This project is my first real kernel driver project, it isn't complete, but i think it demonstrate my knowledge in kernel dev
