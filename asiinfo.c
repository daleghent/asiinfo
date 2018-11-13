#include <stdio.h>
#include <io.h>
#include "include/ASICamera2.h"

const char* bayer[] = {"RG","BG","GR","GB"};

int  main() {

    int numCams;
    int camNum;
    ASI_CAMERA_INFO ASICameraInfo;

    int numCtrls = 0;
    int ctrlNum;
    ASI_CONTROL_CAPS ControlCaps;

    ASI_BOOL bAuto = ASI_FALSE;
    long int value;

    if ((numCams = ASIGetNumOfConnectedCameras()) <= 0) {
        printf("no cameras connected\n");
        return -1;
    }

	for (camNum = 0; camNum < numCams; camNum++) {

		ASIGetCameraProperty(&ASICameraInfo, camNum);
		printf("%d %s\n", camNum, ASICameraInfo.Name);

		if (ASIOpenCamera(camNum) != ASI_SUCCESS) {
			printf("OpenCamera error,are you root?\n");
			continue;
		}

		ASIInitCamera(camNum);

		printf("%s information\n", ASICameraInfo.Name);

		printf("\tresolution: %ld x %ld\n", ASICameraInfo.MaxWidth, ASICameraInfo.MaxHeight);
		if (ASICameraInfo.IsColorCam)
			printf("\ttype: color (bayer pattern:%s)\n", bayer[ASICameraInfo.BayerPattern]);
		else
			printf("\ttype: mono\n");

		ASIGetNumOfControls(camNum, &numCtrls);

		for (ctrlNum = 0; ctrlNum < numCtrls; ctrlNum++)
		{
			memset(&ControlCaps, 0, sizeof(struct _ASI_CONTROL_CAPS));

			ASIGetControlCaps(camNum, ctrlNum, &ControlCaps);
			printf("\tcontrol: %s, type %d\n", ControlCaps.Name, ControlCaps.ControlType);
			printf("\t\twriteable: %s\n", (ControlCaps.IsWritable) ? "yes" : "no");
			printf("\t\tauto supported: %s\n", (ControlCaps.IsAutoSupported) ? "yes" : "no");
			printf("\t\tminimum value: %ld\n", ControlCaps.MinValue);
			printf("\t\tmaximum value: %ld\n", ControlCaps.MaxValue);
			printf("\t\tdefault value: %ld\n", ControlCaps.DefaultValue);
			printf("\t\tcurrent value: ");

			if (ASIGetControlValue(camNum, ControlCaps.ControlType, &value, &bAuto) == ASI_SUCCESS)
				printf("%ld (auto enabled: %s)\n", value, (bAuto) ? "yes" : "no");
			else
				printf("error!\n");

			printf("\t\tdescription: %s\n", ControlCaps.Description);
		}

		ASICloseCamera(camNum);
	}

    return 1;
}
