#include "borderLineDetection.h"

std::map<std::string, std::vector<cv::Scalar>> loadColorMapRoadMarkings() {
    // Scalars are in (H, S, V) format
    // Each color contains a lower and upper end
    return {
        {"WHITE", {{87, 0, 203}, {125, 48, 241}}},
        { "YELLOW", {{0, 0, 128}, {43, 200, 247}}}
    };
}


void findRoadMarkings() {
    // Collect all sample files
    std::vector<cv::String> files;
    cv::glob(BORDER_IMAGES_PATH, files, false);
    std::map<std::string, std::vector<cv::Scalar>> colorMap = loadColorMapRoadMarkings();

    std::size_t i = 0;
    for (const auto& file : files) {
        cv::Mat image = cv::imread(files[i]);
        std::cout << "** Processing sample " << std::string(file) << " **\n";
        cv::Mat hsv;

        // Generate source points to birds eye view
        cv::Point2f destinationVertices[4] = {
                cv::Point(0, 0),
                cv::Point(WIDTH, 0),
                cv::Point(WIDTH, HEIGHT),
                cv::Point(0, HEIGHT)
        };

        // Respective mapping points, size of image
        cv::Point2f sourceVertices[4] = {
                cv::Point(0, 300),
                cv::Point(720, 300),
                cv::Point(720, 1000),
                cv::Point(0, 1000)
        };

        cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(sourceVertices, destinationVertices);
        cv::Mat birdsEyeView(WIDTH, HEIGHT, CV_8UC3), invertedPerspectiveMatrix;
        cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);

        cv::warpPerspective(image, birdsEyeView, perspectiveMatrix, birdsEyeView.size(),  cv::INTER_LINEAR, cv::BORDER_CONSTANT);
        cv::imshow("bev", birdsEyeView);

        // Color filtering
        cv::cvtColor(birdsEyeView, hsv, cv::COLOR_BGR2HSV);
        int index = 0;
        std::vector<cv::Mat> rangedImages(colorMap.size());
        // Filter for specific image colors
        for (auto const& [key, value] : colorMap) {
            cv::inRange(hsv, value[0], value[1], rangedImages[index]);
            index++;
        }

        cv::Mat maskedImage = rangedImages[0].clone();
        for (int t = 1; t < (int)colorMap.size(); t++) {
            cv::add(maskedImage, rangedImages[t], maskedImage);
        }

        // Try to improve gaps
        cv::GaussianBlur(maskedImage, maskedImage, cv::Size(9, 9), 0);
        cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
        cv::dilate(maskedImage, maskedImage, kernel);
        cv::erode(maskedImage, maskedImage, kernel);

        cv::morphologyEx(maskedImage, maskedImage, cv::MORPH_CLOSE, kernel);
        cv::imshow("Maks", maskedImage);

        std::vector<cv::Vec4i> linesP; // will hold the results of the detection
        HoughLinesP(maskedImage, linesP, 1, CV_PI/180, 50, 50, 10); // runs the actual detection
        // Draw the lines
//        cv::perspectiveTransform(linesP, linesPP, invertedPerspectiveMatrix);

        cv::Mat matrix(WIDTH, HEIGHT, CV_8UC3);
        for (auto l : linesP) {
            line(matrix, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
                 cv::Scalar(0,0,255), 1, cv::LINE_AA
            );
        }

        cv::Mat newMat(WIDTH, HEIGHT, CV_8UC3, cv::Scalar(0, 0, 0));
        cv::Mat newMatr;
        cv::warpPerspective(matrix, newMat, invertedPerspectiveMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
        cv::inRange(newMat, cv::Scalar(0, 0, 155), cv::Scalar(0, 0, 255), newMatr);
        cv::add(image, newMat, image);

        cv::imshow("Final image", image);
        cv::waitKey();
        i++;
    }
}
