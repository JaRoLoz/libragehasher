#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <sha1.h>
#include "keys.hpp"

#define BLOCK_LENGTH (1024 * 1024)
#define ALIGNMENT 16

class HasherPool
{
public:
    HasherPool(const uint8_t* source, const size_t sourceSize);
    ~HasherPool();
	void SearchHashes(uint8_t* output, const uint8_t *keyHashes, const size_t keyCount, const size_t keySize);
    uint8_t* SearchHashesH(const uint8_t* keyHashes, const size_t keyCount, const size_t keySize);

private:
    void SearchHashesThread(size_t index, uint8_t* output, const uint8_t* keyHashes, const size_t keyCount, const size_t keySize);
    const uint8_t *m_Source;
	const size_t m_SourceSize;
};