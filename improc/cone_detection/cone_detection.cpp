#include "cone_detection.h"


/**
 * @brief Return a list of masks  for colors of interest
 * Each color contains a list of scalars in 
 * {(Hmin Smin Vmin) (Hmax, Smax, Vmax)} format
 */
const std::vector<std::pair<std::string, std::vector<
        std::pair<cv::Scalar, cv::Scalar>>>> get_color_masks() {
    return {
        {"RED", {
            {{  0, 120,  80}, { 15, 255, 255}},
            {{159, 135,  90}, {179, 255, 160}},
        }},
        {"YELLOW", {
            {{ 16, 188, 116}, { 63, 255, 238}},
        }},
        {"BLUE", {
            {{ 88, 134, 125}, {132, 236, 200}},
        }},
    };
}


cv::Mat mask_image(cv::Mat image, std::vector<std::pair<
        cv::Scalar, cv::Scalar>> masks) {
    std::vector<cv::Mat> masked_images(masks.size());
    for(int i = 0; i < masks.size(); i++) {
        cv::inRange(image, masks[i].first, masks[i].second, masked_images[i]);
    }
    cv::Mat masked_image = masked_images[0].clone();
    for(int i = 1; i < masks.size(); i++) {
        cv::add(masked_image, masked_images[i], masked_image);
    }
    return masked_image;
}


std::vector<std::vector<cv::Point>> get_contours_from_image(cv::Mat current,
        cone_info &cones, std::string color) {
    auto gaussian_blur_kernel = cv::Size(3, 3);
    cv::Mat gaussian_blur, canny, dilated, eroded;

    cv::GaussianBlur(current, gaussian_blur, gaussian_blur_kernel, 3, 0);
    cones.images.push_back({"4_" + color + "_GAUSSIAN", gaussian_blur});
    current = gaussian_blur;

    cv::Canny(current, canny, CANNY_THRESH_LOW, CANNY_THRESH_HIGH);
    cones.images.push_back({"5_" + color + "_CANNY", canny});
    current = canny;

    auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(current, dilated, kernel);
    cones.images.push_back({"6_" + color + "_DILATED", dilated});
    current = dilated;

    cv::erode(current, eroded, kernel);
    cones.images.push_back({"7_" + color + "_ERODED", eroded});
    current = eroded;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(current, contours, hierarchy, cv::RETR_EXTERNAL, 
            cv::CHAIN_APPROX_SIMPLE);
    return contours;
}


cone_info find_cones(cv::Mat image) {
    cone_info cones;
    cones.images.push_back({"1_ORIGINAL", image});

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cones.images.push_back({"2_HSV", hsv});

    auto color_masks = get_color_masks();
    std::vector<cv::Mat> masked_images(color_masks.size());
    for(int i = 0; i < color_masks.size(); i++) {
        masked_images[i] = mask_image(hsv, color_masks[i].second);
        cones.images.push_back({"3_" + color_masks[i].first + "_MASK", 
                masked_images[i]}); 
    }
    
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    for(int i = 0; i < masked_images.size(); i++) {
        auto contours = get_contours_from_image(
                masked_images[i], cones, color_masks[i].first);
        cv::Mat final_image = find_cones_in_contours(image, contours);
        cones.images.push_back({"FINAL", final_image});
    }
    return cones;
}




/**
 * @brief Search for cones in sampled images
 * @param void
 * @return None
 */
//void cone_detection_from_sample_images() {
//    for (int s = 1; s <= SAMPLES; s++) {
//        std::cout << "** Processing sample " << std::to_string(s) << " **\n";
//        cv::Mat image = cv::imread(SOURCE_FOLDER + std::to_string(s) + ".jpg");
//        auto *sampledImage = new Image(image, DESTINATION_FOLDER + std::to_string(s));
//        auto processBegin = std::chrono::high_resolution_clock::now();
//
//        // Process image to find candidate edges
//        // getBorderedImage(sampledImage);
//        auto processedImages = std::chrono::high_resolution_clock::now();
//
//        // Find all cone contours
//        searchContours(sampledImage);
//        auto processedContours = std::chrono::high_resolution_clock::now();
//        std::cout << sampledImage->cont.pointingUpContours.size() << " cone(s) found\n";
//
//        // Save images on disk
//        sampledImage->saveImagesOnDisk(STEP_BY_STEP);
//        auto processEnd = std::chrono::high_resolution_clock::now();
//
//        // Time processing
//        std::chrono::duration<double, std::milli> processImage = processedImages - processBegin;
//        std::chrono::duration<double, std::milli> findContours = processedContours - processedImages;
//        std::chrono::duration<double, std::milli> saveOnDisk = processEnd - processedContours;
//        double coneDetection = processImage.count() + findContours.count();
//        double processTotal = coneDetection + saveOnDisk.count();
//
//        std::cout << "Processing image: " << processImage.count() << " ms\n";
//        std::cout << "Finding contours: " << findContours.count() << " ms\n";
//        std::cout << "Cone detection:   " << coneDetection << " ms\n";
//        std::cout << "Saving files:     " << saveOnDisk.count() << " ms\n";
//        std::cout << "Iteration time:   " << processTotal << " ms\n";
//        std::cout << "\n";
//    }
//}
//
///**
// * @brief Find cones using default camera input
// * @param void
// * @return None
// */
//void coneDetectionVideo() {
//    Camera camera;
//    int frame = 0;
//    while(true) {
//        auto processBegin = std::chrono::high_resolution_clock::now();
//
//        camera.update_frame(); // Read camera input
//        auto *cameraFrame = new Image(camera.get_corrected_frame(), CONTINUOUS_DESTINATION_FOLDER + std::to_string(frame));
//        // :getBorderedImage(cameraFrame); // Process frame
//        searchContours(cameraFrame);   // Find cone contours in frame
//        cv::imshow("ConeDetection", cameraFrame->finalImage);
//        cv::waitKey(1);
//
//        if (!(frame % FRAMES_TO_SAVE)) {
//            std::cout << "Saving frame" << std::endl;
//            cameraFrame->saveImagesOnDisk(true);
//        }
//
//        delete cameraFrame;
//        auto processEnd = std::chrono::high_resolution_clock::now();
//        auto process = processEnd - processBegin;
//        std::cout << "fps " << 1000 / (process.count()) << "\n";
//        frame++;
//    }
//}
//
