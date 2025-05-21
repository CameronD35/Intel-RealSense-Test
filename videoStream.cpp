#include <iostream> // Used for std::cout and std::endl
// #include <iomanip> // Used in the RealSense demo, but unecessary here for now

#include <librealsense2/rs.hpp> // RealSense library
// #include "example.hpp" // Helper functions for UI elements (unnecessary)

int main(int argc, char * argv[]) try {

    std::cout << "hello world" << std::endl;

    // Creates a context for the RealSense camera
    rs2::context context;

    // Creates a default configuation for the RealSense camera
    rs2::config config;

    // Adds the ability to record frames and stream to a file
    config.enable_record_to_file("D:\\Software\\Intel Realsense\\bagFiles\\PLEASEWORK.bag");

    // Creates a shared pointer to a pipeline
    // I don't believe this is completely necessary since I believe this is only needed for multiple devices
    // However, working with this shared pointer doesn't add too much complexity
    auto pipe = std::make_shared<rs2::pipeline>();

    // Starts (opens) the pipeline with the configuration created above
    pipe -> start(config);

    // Grabs the active devices from the pipeline
    // For now we don't use this, but it will likely come in handy later
    rs2::device camera = pipe -> get_active_profile().get_device();

    // Waits for a frame to become available and captures it
    // I'm assuming 30 fps for this camera, so this should output around 2 seconds
    for (int i = 0; i < 60; i++) {

        auto frames = pipe -> wait_for_frames();
        
        std::cout << i << std::endl;

    }

    // Clean up and stop the pipeline before app termination
    pipe -> stop();

    return EXIT_SUCCESS;

// Prints an error related to the RealSense library
} catch (const rs2::error & e){

    // Ignore these goofy outputs
    std::cout << "OR HERE" << std::endl;

    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;

    return EXIT_FAILURE;

// Prints an error related to the overall program
} catch (const std::exception & e) {

    // Trust me, I was trippin when I was writing this
    std::cout << "IS IT GETTING HERE" << std::endl;

    std::cerr << e.what() << std::endl;
    
    return EXIT_FAILURE;

}