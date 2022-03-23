#include "borderLineDetection.h"

std::map<std::string, std::vector<cv::Scalar>> loadColorMapRoadMarkings() {
    // Scalars are in (H, S, V) format
    // Each color contains a lower and upper end
    return {
//        { "WHITE",  {{0, 0, 207},    {110, 22, 255}}},
        {"WHITE", {{87, 0, 203}, {125, 48, 241}}},
        { "YELLOW", {{0, 0, 128}, {43, 200, 247}}}
    };
}

std::vector<cv::Point2f> slidingWindow(cv::Mat image, cv::Rect window) {
        std::vector<cv::Point2f> points;
        const cv::Size imgSize = image.size();
        bool shouldBreak = false;

        while (true) {
            float currentX = window.x + window.width * 0.5f;
            cv::Mat roi = image(window); //Extract region of interest
            std::vector<cv::Point2f> locations;

            findNonZero(roi, locations); //Get all non-black pixels. All are white in our case
            float avgX = 0.0f;

            for (int i = 0; i < locations.size(); ++i) { //Calculate average X position         {
                float x = locations[i].x;
                avgX += window.x + x;
            }

        avgX = locations.empty() ? currentX : avgX / locations.size();

        cv::Point point(avgX, window.y + window.height * 0.5f);
        points.push_back(point);

        //Move the window up         window.y -= window.height;
        window.y -= window.height;

        //For the uppermost position         if (window.y < 0)
        if (window.y < 0) {
            window.y = 0;
            shouldBreak = true;
        }

        //Move x position
        window.x += (point.x - currentX);

        //Make sure the window doesn't overflow, we get an error if we try to get data outside the matrix
        if (window.x < 0)
            window.x = 0;

        if (window.x + window.width >= imgSize.width)
            window.x = imgSize.width - window.width - 1;

        if (shouldBreak)
            break;
    }

    return points;
}

void findRoadMarkings() {
    /*
     * TODO
     * Change image format to birds eye view
     * Filter for road markings
     * Delimit them and save their information in a structure
     */

    // Collect all sample files
    std::vector<cv::String> files;
    cv::glob(BORDER_IMAGES_PATH, files, false);
    std::map<std::string, std::vector<cv::Scalar>> colorMap = loadColorMapRoadMarkings();

    std::size_t i = 0;
    for (const auto& file : files) {
        cv::Mat image = cv::imread(files[i]);
//        cv::imshow("Original image", image);
//        cv::waitKey();
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

        std::cout << "yas" << std::endl;
        cv::Mat maskedImage = rangedImages[0].clone();
        for (int i = 1; i < (int)colorMap.size(); i++) {
            cv::add(maskedImage, rangedImages[i], maskedImage);
        }


        // Try to improve gaps
        cv::GaussianBlur(maskedImage, maskedImage, cv::Size(9, 9), 0);
        cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
        cv::dilate(maskedImage, maskedImage, kernel);
        cv::erode(maskedImage, maskedImage, kernel);

        cv::morphologyEx(maskedImage, maskedImage, cv::MORPH_CLOSE, kernel);

        cv::imshow("Maks", maskedImage);
        cv::waitKey();
        const int thresholdVal = 150;
        cv::threshold(maskedImage, maskedImage, thresholdVal, 255, cv::THRESH_BINARY);
        std::vector<cv::Point2f> points = slidingWindow(maskedImage, cv::Rect(0, 420, 120, 60));

        std::vector<cv::Point> allPts; //Used for the end polygon at the end.
        std::vector<cv::Point2f> outPts;
        perspectiveTransform(points, outPts, invertedPerspectiveMatrix); //Transform points back into original image space
//Draw the points onto the out image
        for (int i = 0; i < outPts.size() - 1; ++i)
        {
            line(image, outPts[i], outPts[i + 1], cv::Scalar(255, 0, 0), 3);
            allPts.push_back(cv::Point(outPts[i].x, outPts[i].y));
        }

        allPts.push_back(cv::Point(outPts[outPts.size() - 1].x, outPts[outPts.size() - 1].y));

        cv::Mat out;
        cvtColor(maskedImage, out, cv::COLOR_GRAY2BGR); //Conver the processing image to color so that we can visualise the lines
        for (int i = 0; i < points.size() - 1; ++i) //Draw a line on the processed image
            line(out, points[i], points[i + 1], cv::Scalar(255, 0, 0));

//Sliding window for the right side
            points = slidingWindow(maskedImage, cv::Rect(520, 420, 120, 60));
            perspectiveTransform(points, outPts, invertedPerspectiveMatrix);

//Draw the other lane and append points
        for (int i = 0; i < outPts.size() - 1; ++i)
        {
            line(image, outPts[i], outPts[i + 1], cv::Scalar(0, 0, 255), 3);
            allPts.push_back(cv::Point(outPts[outPts.size() - i - 1].x, outPts[outPts.size() - i - 1].y));
        }

        allPts.push_back(cv::Point(outPts[0].x - (outPts.size() - 1) , outPts[0].y));

        for (int i = 0; i < points.size() - 1; ++i)
            line(out, points[i], points[i + 1], cv::Scalar(0, 0, 255));

//Create a green-ish overlay
        std::vector<std::vector<cv::Point>> arr;
        arr.push_back(allPts);
        cv::Mat overlay = cv::Mat::zeros(image.size(), image.type());
        fillPoly(overlay, arr, cv::Scalar(0, 255, 100));
        addWeighted(image, 1, overlay, 0.5, 0, image); //Overlay it
//        std::cout << "yas" << std::endl;
        imshow("Output", image);
        cv::waitKey(0);
        i++;
    }
}
