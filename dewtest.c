#include <stdio.h>
#include <io.h>
#include "include/ASICamera2.h"

const char* bayer[] = {"RG","BG","GR","GB"};

int  main() {

    int numCams;
    int camNum;
    ASI_CAMERA_INFO ASICameraInfo;
    ASI_CONTROL_CAPS ControlCaps;
    ASI_BOOL bAuto = ASI_FALSE;
    long int value;
    int rv;

    if ((numCams = ASIGetNumOfConnectedCameras()) <= 0) {
        printf("no cameras connected\n");
        return -1;
    }

	for (camNum = 0; camNum < numCams; camNum++) {

		ASIGetCameraProperty(&ASICameraInfo, camNum);
		printf("%d %s\n", camNum, ASICameraInfo.Name);

		if (ASIOpenCamera(camNum) != ASI_SUCCESS) {
			printf("OpenCamera error, are you root?\n");
			continue;
		}

		ASIInitCamera(camNum);

		printf("%s information\n", ASICameraInfo.Name);

		printf("\tresolution: %ld x %ld\n", ASICameraInfo.MaxWidth, ASICameraInfo.MaxHeight);
		if (ASICameraInfo.IsColorCam)
			printf("\ttype: color (bayer pattern:%s)\n", bayer[ASICameraInfo.BayerPattern]);
		else
			printf("\ttype: mono\n");

		rv = ASIGetControlCaps(camNum, ASI_ANTI_DEW_HEATER, &ControlCaps);

		if (rv == ASI_SUCCESS) {
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
		} else {
			printf("\tDew Heater is not present. return code = %d\n", rv);
		}

		ASICloseCamera(camNum);
	}

    return 1;
}
