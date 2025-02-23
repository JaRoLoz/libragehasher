#include "HasherPool.hpp"

HasherPool::HasherPool(const uint8_t* source, const size_t sourceSize) :
	m_Source(source),
	m_SourceSize(sourceSize)
{
}

HasherPool::~HasherPool()
{
}

uint8_t* HasherPool::SearchHashesHeap(const uint8_t* keyHashes, const size_t keyCount, const size_t keySize)
{
	uint8_t* output = new uint8_t[keyCount * keySize];
	this->SearchHashes(output, keyHashes, keyCount, keySize);
	return output;
}

void HasherPool::SearchHashes(uint8_t* output, const uint8_t* keyHashes, const size_t keyCount, const size_t keySize)
{
	std::vector<std::thread> threads;
	const size_t numCpus = std::thread::hardware_concurrency();

	for (size_t i = 0; i < numCpus; i++)
	{
		threads.push_back(std::thread([=]() { this->SearchHashesThread(i, output, keyHashes, keyCount, keySize); }));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void HasherPool::SearchHashesThread
(
	size_t index,
	uint8_t* output,
	const uint8_t* keyHashes,
	const size_t keyCount,
	const size_t keySize
)
{
	const size_t numCpus = std::thread::hardware_concurrency();
	const size_t blockSize = this->m_SourceSize / numCpus;

	for (size_t i = 0; i < (blockSize / ALIGNMENT); i++)
	{
		const uint8_t* startPtr = this->m_Source + (blockSize * index) + (i * ALIGNMENT);
		const uint8_t* endPtr = startPtr + keySize;

		if (endPtr > (this->m_Source + this->m_SourceSize))
		{
			break;
		}

		uint8_t hash[SHA1_HASH_SIZE];
		SHA1_CTX sha1;
		SHA1Init(&sha1);
		SHA1Update(&sha1, startPtr, keySize);
		SHA1Final(hash, &sha1);

		for (size_t keyIndex = 0; keyIndex < keyCount; keyIndex++)
		{
			const uint8_t* keyPtr = keyHashes + (keyIndex * SHA1_HASH_SIZE);
			if (memcmp(hash, keyPtr, SHA1_HASH_SIZE) == 0)
			{
				memcpy(output + (keyIndex * keySize), startPtr, keySize);
			}
		}
	}
}