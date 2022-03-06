#include "cameraCalibration.h"

void calibrateCamera() {
    // Collect all sample files
    std::vector<cv::String> files;
    cv::glob(SAMPLE_IMAGES_PATH, files, false);

    // Define world and image frames
    cv::Size patternSize(CHESSBOARD_HEIGHT - 1, CHESSBOARD_WIDTH - 1);
    std::vector<std::vector<cv::Point2f>> cameraFrame(files.size());
    std::vector<std::vector<cv::Point3f>> worldFrame;

    // Define world coordinates for points
    std::vector<cv::Point3f> objectPosition;
    for (int i = 1; i < CHESSBOARD_HEIGHT; i++) {
        for (int j = 1; j < CHESSBOARD_WIDTH; j++) {
            objectPosition.emplace_back(i, j, 0);
        }
    }

    std::vector<cv::Point2f> imagePoint;
    // Detect feature points
    std::size_t i = 0;
    for (auto const &file : files) {
        std::cout << "Analyzing " << std::string(file) << "\n";

        cv::Mat img = cv::imread(files[i]);
        cv::Mat gray;

        cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
        bool patternFound = cv::findChessboardCorners(
            gray,
            patternSize,
            cameraFrame[i],
            cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
        );

        // Improve points detected
        if(patternFound){
            cv::cornerSubPix(
                gray, cameraFrame[i],
                cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(
                    cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1
                )
            );
            worldFrame.push_back(objectPosition);
        }

        // Show image with detected chessboard
        cv::drawChessboardCorners(img, patternSize, cameraFrame[i], patternFound);
        cv::imshow("Chessboard " + file, img);
        cv::waitKey(0);
        i++;
    }

    // Calculate intrinsic camera matrix
    cv::Matx33f intrinsicMatrix(cv::Matx33f::eye());
    cv::Vec<float, 5> distortionMatrix(0, 0, 0, 0, 0);

    std::vector<cv::Mat> rotationVectors, translationVectors;
    std::vector<double> stdIntrinsics, stdExtrinsics, perViewErrors;
    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 +
                cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT;
    cv::Size frameSize(1280, 960);

    float error = cv::calibrateCamera(
            worldFrame, cameraFrame, frameSize,
            intrinsicMatrix, distortionMatrix,
            rotationVectors, translationVectors, flags
    );

    std::cout << "Reprojection error = " << error
              << "\nintrinsicMatrix =\n" << intrinsicMatrix
              << "\ndistortionMatrix=\n" << distortionMatrix
              << std::endl;

    // Lens correction interpolation
    cv::Mat mapX, mapY;
    cv::initUndistortRectifyMap(
        intrinsicMatrix, distortionMatrix, cv::Matx33f::eye(), 
        intrinsicMatrix, frameSize, CV_32FC1,
        mapX, mapY
    );

    cv::initUndistortRectifyMap(
        intrinsicMatrix, distortionMatrix, cv::Matx33f::eye(),
        intrinsicMatrix, frameSize, CV_32FC1,
        mapX, mapY
    );

    // Show lens corrected images
    for (auto const &f : files) {
        cv::Mat img = cv::imread(f, cv::IMREAD_COLOR);
        cv::Mat imgUndistorted;
        cv::remap(img, imgUndistorted, mapX, mapY, cv::INTER_LINEAR);

        // Display
        cv::imshow("undistorted image" + std::string(f), imgUndistorted);
        cv::waitKey(0);
    }
}
