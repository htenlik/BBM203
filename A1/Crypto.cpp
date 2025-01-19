#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length) {
    std::vector<int> LSB_array;
    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int dimensions = image.get_width() * image.get_height();

    // 3. Determine the total bits required based on message length.
    int total_bits = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (total_bits > dimensions) {
        throw std::runtime_error("Error: Not enough pixels");
    }

    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    for(int i = 0; i < total_bits; i++) {
        int x = (dimensions - total_bits + i) / image.get_width();
        int y = (dimensions - total_bits + i) % image.get_width();
        LSB_array.push_back(image.get_pixel(x, y) & 1);
    }

    // 6. Extract LSBs from the image pixels and return the result.
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("Error: Invalid LSB array size");
    }

    // 2. Convert each group of 7 bits into an ASCII character.
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        std::bitset<7> bits;
        for (size_t j = 0; j < 7; j++) {
            bits[6-j] = LSB_array[i + j];
        }
        // 3. Collect the characters to form the decrypted message.
        message += static_cast<char>(bits.to_ulong());
    }

    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    for (char c : message) {
        std::bitset<7> bits(c);
        // 2. Collect the bits into the LSB array.
        for (int i = 0; i < 7; i++) {
            LSB_array.push_back(bits[6-i]);
        }
    }

    // 3. Return the array of bits.
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int total_pixels = image.get_width() * image.get_height();
    if(total_pixels < LSB_array.size()) {
        throw std::runtime_error("Error: Not enough pixels");
    }
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int start_pixel = total_pixels - LSB_array.size();

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for(int i = 0; i < LSB_array.size(); i++) {
        int x = (start_pixel + i) / image.get_width();
        int y = (start_pixel + i) % image.get_width();
        image.set_pixel(x, y, (image.get_pixel(x, y))& ~1 | LSB_array[i]);

    }

    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.
    return SecretImage(image);
}
