#include <iostream>
#include <librealsense2/rs.hpp>

int main(int argc, char * argv[]) try {

    std::cout << "hello world" << std::endl;

    rs2::pipeline p;

    p.start();

    while (true) {

        // This camera can produce multiple streams, so this waits until the next set of frames is
        // prepared from various streams
        rs2::frameset frames = p.wait_for_frames();

        // Grabs image depth
        rs2::depth_frame depth = frames.get_depth_frame();

        // Grabs dimensions of depth frame
        float width = depth.get_width();
        float height = depth.get_height();

        // Use the get_distance function to get distance at a specific pixel
        float center_distance = depth.get_distance(width / 2, height / 2);

        std::cout << "The camera is facing an object " << center_distance << " meters away." << std::endl;

    }

    return EXIT_SUCCESS;

} catch (const rs2::error & e){

    std::cout << "OR HERE" << std::endl;

    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;

    return EXIT_FAILURE;

} catch (const std::exception & e) {

    std::cout << "IS IT GETTING HERE" << std::endl;

    std::cerr << e.what() << std::endl;
    
    return EXIT_FAILURE;

}