#include "VulkanTester.h"

# include  <cstdlib>
# include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN


int main()
{
    Sanbox proto{};

        try
        {
            proto.run();
        } catch (const std::exception &e)
        {
            std::cerr << e.what();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;

}
