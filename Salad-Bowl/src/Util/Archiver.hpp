// A simple wrapper around the libzip(zlib) library

#pragma once

#include "Salad/Core/Log.h"
#include "array"
#include "zip.h"

namespace Salad::Util {

	typedef zip* Archive;
	typedef zip_source* ArchiveSource;

	enum ArchiverOpenFlags_ {
		ArchiverOpenFlags_None                          = 0,						// No flag
		ArchiverOpenFlags_Create                        = ZIP_CREATE,				// Create the archive if it does not exist
		ArchiverOpenFlags_Excl                          = ZIP_EXCL,					// Error if the archive already exists
		ArchiverOpenFlags_Truncate                      = ZIP_TRUNCATE,				// If archive exists, ignore its current contents. In other words, handle it the same way as an empty archive.
		ArchiverOpenFlags_CheckCons                     = ZIP_CHECKCONS,			// Perform additional stricter consistency checks on the archive, and error if they fail.
	};

	enum ArchiverFlags_ {
		ArchiverFlags_IgnoreCase 						= ZIP_FL_NOCASE,			// Ignore case on name lookup
		ArchiverFlags_IgnoreDirectory					= ZIP_FL_NODIR,				// Ignore directory component
		ArchiverFlags_Compressed						= ZIP_FL_COMPRESSED,		// Read compressed data
		ArchiverFlags_Unchanged							= ZIP_FL_UNCHANGED,			// Use original data
		ArchiverFlags_Recompress						= ZIP_FL_RECOMPRESS,		// Force recompression of data
		ArchiverFlags_Encrypted							= ZIP_FL_ENCRYPTED,			// Read encrypted data (implies ArchiverFlags_Compressed)
		ArchiverFlags_EncodingGuess						= ZIP_FL_ENC_GUESS,			// Guess string encoding (default)
		ArchiverFlags_EncodingRaw						= ZIP_FL_ENC_RAW,			// Get unmodified string
		ArchiverFlags_EncodingStrict					= ZIP_FL_ENC_STRICT,		// Follow specification strictly
		ArchiverFlags_Local								= ZIP_FL_LOCAL,				// In local header
		ArchiverFlags_Central							= ZIP_FL_CENTRAL,			// In central directory
		ArchiverFlags_EncodingUTF8						= ZIP_FL_ENC_UTF_8,			// String is UTF8 encoded
		ArchiverFlags_EncodingCP437						= ZIP_FL_ENC_CP437,			// String is CP437 encoded
		ArchiverFlags_Overwrite							= ZIP_FL_OVERWRITE,			// zip_file_add: if file with name exists, overwrite (replace) it
	};

	enum ArchiverErrors_ {
		ArchiverErrors_None                             = ZIP_ER_OK,				// No errors
		ArchiverErrors_Multidisk                        = ZIP_ER_MULTIDISK,			// Multi-disk archives are not supported
		ArchiverErrors_Rename                           = ZIP_ER_RENAME,			// Renaming temporary file failed
		ArchiverErrors_Close                            = ZIP_ER_CLOSE,				// Closing zip archive failed
		ArchiverErrors_Seek                             = ZIP_ER_SEEK,				// Seek error
		ArchiverErrors_Read                             = ZIP_ER_READ,				// Read error
		ArchiverErrors_Write                            = ZIP_ER_WRITE,				// Write error
		ArchiverErrors_CRC								= ZIP_ER_CRC,				// CRC error
		ArchiverErrors_ZipClosed						= ZIP_ER_ZIPCLOSED,			// Containing archive was closed
		ArchiverErrors_NoFile							= ZIP_ER_NOENT,				// No such file
		ArchiverErrors_FileExists						= ZIP_ER_EXISTS,			// File already exists
		ArchiverErrors_Open								= ZIP_ER_OPEN,				// Can't open file
		ArchiverErrors_TempOpen							= ZIP_ER_TMPOPEN,			// Failed to create temporary file
		ArchiverErrors_Zlib								= ZIP_ER_ZLIB,				// Zlib error
		ArchiverErrors_Memory							= ZIP_ER_MEMORY,			// Malloc failure
		ArchiverErrors_EntryChanged						= ZIP_ER_CHANGED,			// Entry has been changed
		ArchiverErrors_CompressionNotSupported			= ZIP_ER_COMPNOTSUPP,		// Compression method not supported
		ArchiverErrors_EndOfFile						= ZIP_ER_EOF,				// Premature EOF
		ArchiverErrors_InvalidArgument                  = ZIP_ER_INVAL,				// Invalid argument
		ArchiverErrors_NoArchive						= ZIP_ER_NOZIP,				// Not a zip archive
		ArchiverErrors_Internal                         = ZIP_ER_INTERNAL,			// Internal error
		ArchiverErrors_Inconsistent					    = ZIP_ER_INCONS,			// Zip archive is inconsistent
		ArchiverErrors_Remove							= ZIP_ER_REMOVE,			// Can't remove file
		ArchiverErrors_Deleted							= ZIP_ER_DELETED,			// Entry has been deleted
		ArchiverErrors_EncryptionNotSupported			= ZIP_ER_ENCRNOTSUPP,		// Encryption method not supported
		ArchiverErrors_ReadOnly							= ZIP_ER_RDONLY,			// Read-only archive
		ArchiverErrors_NoPassword						= ZIP_ER_NOPASSWD,			// No password provided
		ArchiverErrors_WrongPassword					= ZIP_ER_WRONGPASSWD		// Wrong password provided
	};

	// TODO: can probably merge ArchiveFileReadBuffer and ArchiveFileWriteBuffer into ArchiveBuffer

	/// <summary>
	/// 
	/// </summary>
	class ArchiveFileReadBuffer {
	
	public:
		ArchiveFileReadBuffer() = delete;
		ArchiveFileReadBuffer(char* data, uint64_t size) : m_Data(data), m_Size(size) {}

		uint64_t getBufferSize() { return m_Size; }
		char* getBuffer() { return m_Data; }
		void freeBuffer() { free(m_Data); }

		bool canRead(uint64_t amount = 1) { return m_Iterator + amount <= m_Size; }

		char readByte() { return m_Data[incrementIterator(1)]; }
		//unsigned char readUByte() {}
		//int16_t readShort() {}
		//uint16_t readUShort() {}
		int32_t readInt() { return *((int*)&m_Data[incrementIterator(4)]); }
		//uint32_t readUInt() {}
		//int64_t readLong() {}
		//uint64_t readULong() {}
		float readFloat() { return *((float*)&m_Data[incrementIterator(4)]); }
		//std::string& readString() {}

	private:
		uint64_t incrementIterator(float amount) {
			uint64_t pos = m_Iterator;
			m_Iterator += amount;
			return pos;
		}

	private:
		char* m_Data = nullptr;
		uint64_t m_Size = 0;
		uint64_t m_Iterator = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class ArchiveFileWriteBuffer {
	
	public:
		ArchiveFileWriteBuffer() = delete;
		ArchiveFileWriteBuffer(const char* filename, uint64_t size) : m_Filename(filename), m_Size(size) { m_Data = (char*)malloc(m_Size); }

		uint64_t getBufferSize() { return m_Size; }
		char* getBuffer() { return m_Data; }
		void freeBuffer() { free(m_Data); }

		const char* getFileName() { return m_Filename; }

		bool canWrite(uint64_t amount = 1) { return m_Iterator + amount <= m_Size; }

		void writeByte(char value) { m_Data[m_Iterator] = value; m_Iterator++; }
		//void writeUByte(unsigned char value) {}
		//void writeShort(int16_t value) {}
		//void writeUShort(uint16_t value) {}
		void writeInt(int32_t value) { memcpy(&m_Data[m_Iterator], &value, sizeof(int)); m_Iterator += 4; }
		//void writeUInt(uint32_t value) {}
		//void writeLong(int64_t value) {}
		//void writeULong(uint64_t value) {}
		void writeFloat(float value) { memcpy(&m_Data[m_Iterator], &value, sizeof(float)); m_Iterator += sizeof(float); }
		//void writeString(const std::string& value) {}

	private:
		const char* m_Filename;
		uint64_t m_Size;
		char* m_Data = nullptr;
		uint64_t m_Iterator = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	inline Archive archiveOpen(const char* archivePath, int openFlags, int* errorCode) {
		Archive archive = zip_open(archivePath, openFlags, errorCode);
		if(archive == nullptr) {
			SLD_CORE_ERROR("Error whilst trying to open archive! Error code: {0}", *errorCode);
			return nullptr;
		}
		return archive;
	}

	inline void archiveClose(Archive archive) {
		zip_close(archive);
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveSource archiveSourceBuffer(Archive archive, const void* data, uint64_t size, int free) {
		ArchiveSource source = zip_source_buffer(archive, data, size, free);
		if (source == nullptr) {
			SLD_CORE_ERROR("Failed to create archive source buffer.'{0}'", zip_strerror(archive));
			return nullptr;
		}
		return source;
	}

	/// <summary>
	/// 
	/// </summary>
	inline uint64_t archiveAddFile(Archive archive, ArchiveFileWriteBuffer buffer, int free) {
		ArchiveSource source = archiveSourceBuffer(archive, buffer.getBuffer(), buffer.getBufferSize(), free);
		if (source == nullptr) return -1;

		uint64_t index = zip_file_add(archive, buffer.getFileName(), source, ZIP_FL_OVERWRITE);
		if(index < 0) {
			SLD_CORE_ERROR("failed to add file to archive.'{0}'", zip_strerror(archive));
		}
		return index;
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveFileReadBuffer archiveReadFile(Archive archive, const char* file) {
		zip_file* zipfile = zip_fopen(archive, file, 0);
		struct zip_stat st;
		zip_stat(archive, file, ZIP_STAT_SIZE, &st);
		char* buffer = (char*) malloc((size_t)st.size);

		uint64_t readBytes = zip_fread(zipfile, buffer, st.size);
		return ArchiveFileReadBuffer(buffer, readBytes);
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveFileWriteBuffer archiveWriteBuffer(Archive archive, const char* filename, uint64_t size) {
		return ArchiveFileWriteBuffer(filename, size);
	}
}
