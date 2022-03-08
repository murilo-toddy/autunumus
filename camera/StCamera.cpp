#include "StCamera.h"

using namespace StApi;
using namespace cv;
using namespace std;

StCamera::StCamera() {
    try {
        // Initialize StApi before using.
        CStApiAutoInit objStApiAutoInit;

        // Create a system object for device scan and connection.
        CIStSystemPtr pIStSystem(CreateIStSystem(StSystemVendor_Sentech));

        // Create a camera device object and connect to first detected device.
        CIStDevicePtr pIStDevice(pIStSystem->CreateFirstIStDevice());

        // Displays the DisplayName of the device.
        cout << "Device=" << pIStDevice->GetIStDeviceInfo()->GetDisplayName() << endl;

        // Create a DataStream object for handling image stream data.
        CIStDataStreamPtr pIStDataStream(pIStDevice->CreateIStDataStream(0));

        // Start the image acquisition of the host side.
        pIStDataStream->StartAcquisition();

        // Start the image acquisition of the camera side.
        pIStDevice->AcquisitionStart();

        // Image buffers for OpenCV.
        Mat inputMat;
        Mat displayMat;

        // A while loop for acquiring data and checking status.
        // Here, the acquisition runs until it reaches the assigned numbers of frames.
        while (pIStDataStream->IsGrabbing())
        {
            // Retrieve the buffer pointer of image data with a timeout of 5000ms.
            CIStStreamBufferPtr pIStStreamBuffer(pIStDataStream->RetrieveBuffer(1000));

            // Check if the acquired data contains image data.
            if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
            {
                // If yes, we create a IStImage object for further image handling.
                IStImage *pIStImage = pIStStreamBuffer->GetIStImage();

                // Display the information of the acquired image data.
                uint64_t blockId = pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID();
                cout << "BlockId=" << blockId
                     << " Size:" << pIStImage->GetImageWidth() << " x " << pIStImage->GetImageHeight()
                     << " First byte =" << (uint32_t)*(uint8_t*)pIStImage->GetImageBuffer() << endl;

                // Check the pixelfomat of the input image.
                const StApi::EStPixelFormatNamingConvention_t ePFNC = pIStImage->GetImagePixelFormat();
                StApi::IStPixelFormatInfo * const pIStPixelFormatInfo = StApi::GetIStPixelFormatInfo(ePFNC);
                if (pIStPixelFormatInfo->IsMono() || pIStPixelFormatInfo->IsBayer())
                {
                    // Check the size of the image.
                    const size_t nImageWidth = pIStImage->GetImageWidth();
                    const size_t nImageHeight = pIStImage->GetImageHeight();
                    int nInputType = CV_8UC1;
                    if (8 < pIStPixelFormatInfo->GetEachComponentTotalBitCount())
                    {
                        nInputType = CV_16UC1;
                    }

                    // Create a OpenCV buffer for the input image.
                    if ((inputMat.cols != (int)nImageWidth) || (inputMat.rows != (int)nImageHeight) || (inputMat.type() != nInputType))
                    {
                        inputMat.create((int)nImageHeight, (int)nImageWidth, nInputType);
                    }

                    // Copy the input image data to the buffer for OpenCV.
                    const size_t dwBufferSize = inputMat.rows * inputMat.cols * inputMat.elemSize() * inputMat.channels();
                    memcpy(inputMat.ptr(0), pIStImage->GetImageBuffer(), dwBufferSize);

                    // Convert the pixelformat if needed.
                    Mat *pMat = &inputMat;
                    if (pIStPixelFormatInfo->IsBayer())
                    {
                        int nConvert = 0;
                        switch (pIStPixelFormatInfo->GetPixelColorFilter())
                        {
                            case(StPixelColorFilter_BayerRG) : nConvert = COLOR_BayerRG2RGB;    break;
                            case(StPixelColorFilter_BayerGR) : nConvert = COLOR_BayerGR2RGB;    break;
                            case(StPixelColorFilter_BayerGB) : nConvert = COLOR_BayerGB2RGB;    break;
                            case(StPixelColorFilter_BayerBG) : nConvert = COLOR_BayerBG2RGB;    break;
                        }
                        if (nConvert != 0)
                        {
                            cvtColor(inputMat, displayMat, nConvert);
                            pMat = &displayMat;
                        }
                    }

                    // Show the image.
                    int num = 300;
                    if (blockId % num == 0) {
                        imwrite("../calibration_images/" + to_string(blockId / num) + ".jpg", *pMat);
                        cout << "Took Photo!!!" << endl;
                    }
                    imshow("Image1", *pMat);
                    waitKey(1);
                }


            }
            else
            {
                // If the acquired data contains no image data.
                cout << "Image data does not exist" << endl;
            }
        }

        // Stop the image acquisition of the camera side.
        pIStDevice->AcquisitionStop();

        // Stop the image acquisition of the host side.
        pIStDataStream->StopAcquisition();
    } catch(const GenICam::GenericException& e) {
        std::cerr << "Error in opening camera" << std::endl << e.GetDescription() << std::endl;
    }
}
