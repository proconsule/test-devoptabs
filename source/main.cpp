#include <string.h>
#include <stdio.h>


#include <string.h>

#include <cstdio>
#include <iostream>
#include <fstream>

#include <switch.h>

#include <string>


#include "smb2fs.h"
#include "sshfs.h"
#include "nfsfs.h"

std::string sshconnectstringh = "ssh://user:pass@192.168.1.10/home/user/";
std::string smbconnectstringh = "smb://user:pass@192.168.1.10/share/";
std::string nfsconnectstringh = "nfs://192.168.1.10/export/";

CSSHFS * testssh;  

int main(int argc, char **argv){
	
	//Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);
	
	SocketInitConfig cfg = *(socketGetDefaultInitConfig());
	cfg.bsd_service_type = BsdServiceType_System;
	socketInitialize(&cfg);
	
	romfsInit();


	
	testssh = new CSSHFS(sshconnectstringh,"ssh0","ssh0");
	
	/* This is the standard way for listing dir */
	
		DIR * dir4 = opendir("ssh0:/");
		struct dirent *ent4;
		while ((ent4 = readdir(dir4)) != nullptr) {
			std::string mypath = "ssh0:/"+std::string(ent4->d_name);
			struct stat st{0};
			printf("%s\n",mypath.c_str());	
			if (stat(mypath.c_str(), &st) == 0) {
				printf("%s %d\n",mypath.c_str(),st.st_size);	
			}
		}
		closedir(dir4);
		
	/* This code speedup the directory listing a lot since no need for stat on each file */
	/* This code was made by averne https://github.com/averne */
		auto *dir = opendir("ssh0:/");    
		if (dir) {
			struct stat st;
			
			auto *reent    = __syscall_getreent();
			auto *devoptab = devoptab_list[dir->dirData->device];

			
			while (true) {
				reent->deviceData = devoptab->deviceData;
				if (devoptab->dirnext_r(reent, dir->dirData, dir->fileData.d_name, &st))
					break;

				auto path = dir->fileData.d_name;
				printf("%s %d\n",dir->fileData.d_name,st.st_size);	
				
				
			}
			closedir(dir);
		}

	
		FILE * testfile = fopen("ssh0:/test.txt","rb");
		if(testfile){
			char buffer[64];
			fseek(testfile,10,SEEK_SET);
			
			int numread = fread(buffer, sizeof(char), sizeof(buffer), testfile);
			printf("NUMREAD: %d\n",numread);
			printf("%s\n",buffer);
		}
		
		testfile = fopen("ssh0:/test.txt","rb");
		if(testfile){
			char buffer[64];
			fseek(testfile,10,SEEK_SET);
			
			int numread = fread(buffer, sizeof(char), sizeof(buffer), testfile);
			printf("NUMREAD: %d\n",numread);
			printf("%s\n",buffer);
		}
	
    while(appletMainLoop())
    {
		
		
		

        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }
	
	//delete testsmb2;
	//delete testssh;

    consoleExit(NULL);
    return 0;
	
}