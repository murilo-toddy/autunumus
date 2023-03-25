#include "calibrate_camera.h"


cv::Mat analyze_image(int index, cv::String file, cv::Size pattern_size, 
            const std::vector<cv::Point3f> &object_position,
            std::vector<std::vector<cv::Point2f>> &camera_frame,
            std::vector<std::vector<cv::Point3f>> &world_frame
        ) {

    cv::Mat image = cv::imread(file);
    cv::Mat gray;

    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    bool pattern_found = cv::findChessboardCorners(gray, pattern_size, 
            camera_frame[index], cv::CALIB_CB_ADAPTIVE_THRESH + 
            cv::CALIB_CB_NORMALIZE_IMAGE);

    // Improve points detected
    if(pattern_found){
        cv::cornerSubPix(gray, camera_frame[index], cv::Size(11, 11), 
                cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + 
                cv::TermCriteria::MAX_ITER, 30, 0.1));
        world_frame.push_back(object_position);
    }

    // Show image with detected chessboard
    cv::drawChessboardCorners(image, pattern_size, camera_frame[index], 
            pattern_found);
    
   return image;
}


void calibrate_camera() {
    // Collect all sample files
    std::vector<cv::String> files = file::load_images_from_query(SAMPLE_IMAGES_PATH);
    std::vector<cv::Mat> processed_images(files.size());

    // Define world and image frames
    cv::Size pattern_size(CHESSBOARD_HEIGHT - 1, CHESSBOARD_WIDTH - 1);
    std::vector<std::vector<cv::Point2f>> camera_frame(files.size());
    std::vector<std::vector<cv::Point3f>> world_frame;

    // Define world coordinates for points
    std::vector<cv::Point2f> image_point;
    std::vector<cv::Point3f> object_position;
    for (int i = 1; i < CHESSBOARD_HEIGHT; i++) {
        for (int j = 1; j < CHESSBOARD_WIDTH; j++) {
             object_position.emplace_back(i, j, 0);
        }
    }

    // Detect feature points
    for(int i = 0; i < files.size(); i++) {
        if(DEBUG) std::cout << "Analyzing " << std::string(files[i]) << "\n";
        cv::Mat image = analyze_image(i, files[i], pattern_size,
                object_position, camera_frame, world_frame);

        if(SAVE_IMAGES) {
            file::save_opencv_matrix_to_file(OUTPUT_PATH, 
                    std::string(files[i]), image);
        }
    }

    // Calculate intrinsic camera matrix
    cv::Matx33f intrinsic_matrix(cv::Matx33f::eye());
    cv::Vec<float, 5> distortion_matrix(0, 0, 0, 0, 0);

    std::vector<cv::Mat> rotation_vectors, translation_vectors;
    std::vector<double> std_intrinsics, std_extrinsics, per_view_errors;
    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 +
                cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT;
    cv::Size frame_size(IMAGE_WIDTH, IMAGE_HEIGHT);

    double error = cv::calibrateCamera(world_frame, camera_frame, frame_size, 
            intrinsic_matrix, distortion_matrix, rotation_vectors, 
            translation_vectors, flags);

    if(DEBUG) {
        std::cout << "Reprojection error = " << error
            << "\nintrinsic_matrix =\n" << intrinsic_matrix
            << "\ndistortion_matrix=\n" << distortion_matrix
            << std::endl;
    }

    if(SAVE_MATRIX) {
        file::save_array_to_file(CORRECTION_MATRIX_PATH, "distortion.mat", 5,
                distortion_matrix.val);
        file::save_array_to_file(CORRECTION_MATRIX_PATH, "intrinsic.mat", 9,
                intrinsic_matrix.val);
        if(DEBUG) std::cout << "Matrices updated\n";
    }
    
    // Lens correction interpolation
    cv::Mat map_x, map_y;
    cv::initUndistortRectifyMap(intrinsic_matrix, distortion_matrix, 
            cv::Matx33f::eye(), intrinsic_matrix, frame_size, CV_32FC1,
            map_x, map_y);

    // Save lens corrected images
    for (int i = 0; i < files.size(); i++) {
        cv::String file = files[i];
        cv::Mat image = cv::imread(file, cv::IMREAD_COLOR);
        cv::Mat image_undistorted;
        cv::remap(image, image_undistorted, map_x, map_y, cv::INTER_LINEAR);

        if(SAVE_IMAGES) {
            std::string image_name = std::to_string(i) + " corrected.jpg";
            file::save_opencv_matrix_to_file(OUTPUT_PATH, image_name, 
                    image_undistorted);
            if(DEBUG) std::cout << image_name << " saved\n";
        }
    }
}


int main (int, char**) {
    if(SAVE_IMAGES) { file::create_folder(OUTPUT_PATH); }
    calibrate_camera();
    return 0;
}

