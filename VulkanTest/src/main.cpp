import VulkanApp;

import <cstdlib>;
import <iostream>;

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
