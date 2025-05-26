#ifndef UTILITIES_H
#define UTILITIES_H

// indices of Queue families (if they exist)
struct QueueFamilyIndices {
    int graphicsFamily = -1; // Location of the graphics queue

    // Check if the queue family indices are valid
    bool isValid() {
        return graphicsFamily >= 0;
    }
};

#endif //UTILITIES_H
