/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#ifndef _MP3_TAG_H
#define _MP3_TAG_H

#include <OS.h>
#include <string.h>
#include <TypeConstants.h>
#include <File.h>

const uint32 WRONG_MIME_TYPE='mime';

class Mp3Tag
{
public:
	Mp3Tag(entry_ref *file_ref);
	~Mp3Tag();

	uint32 InitCheck();
	
	void SetMime(); // sets the MIME-type to "audio/x-mpeg"

	char *Artist();
	char *Album();
	char *Year();
	char *Title();
	char *Comment();
	uint8 Genre();
	void SetArtist(char *artist);
	void SetAlbum(char *artist);
	void SetYear(char *artist);
	void SetComment(char *comment);
	void SetTitle(char *artist);
	void SetGenre(uint8 index);
	void WriteToDisk();
private:
	void strncpy_space(char *source,char *target_space,char *target_null,uint8 size);

	uint32 m_init;

	BFile m_file;
	struct tag_struct
	{
		char header[3];
		char title[30];
		char artist[30];
		char album[30];
		char year[4];
		char comment[30];
		uint8 genre;
	};
	tag_struct m_tag_data;
	char m_year_string[5], m_title_string[31], m_artist_string[31], m_album_string[31], m_comment_string[31];
};

// inline definitions

inline void Mp3Tag::SetMime()
{
	const char *mime_string = "audio/x-mpeg";
	m_file.WriteAttr("BEOS:TYPE",B_STRING_TYPE,0,mime_string,strlen(mime_string)+1);
}

inline char *Mp3Tag::Title()
{
	return m_title_string;
}

inline char *Mp3Tag::Artist()
{
	return m_artist_string;
}

inline char *Mp3Tag::Album()
{
	return m_album_string;
}

inline char *Mp3Tag::Year()
{
	return m_year_string;
}

inline char *Mp3Tag::Comment()
{
	return m_comment_string;
}

inline uint8 Mp3Tag::Genre()
{
	return m_tag_data.genre;
}

inline void Mp3Tag::SetGenre(uint8 index)
{
	m_tag_data.genre = index;
}

inline void Mp3Tag::SetTitle(char *title)
{
	strncpy_space(title, m_tag_data.title, m_title_string, sizeof(m_tag_data.title));
}

inline void Mp3Tag::SetArtist(char *artist)
{
	strncpy_space(artist, m_tag_data.artist, m_artist_string, sizeof(m_tag_data.artist));
}

inline void Mp3Tag::SetAlbum(char *album)
{
	strncpy_space(album, m_tag_data.album, m_album_string, sizeof(m_tag_data.album));
}

inline void Mp3Tag::SetYear(char *year)
{
	strncpy_space(year, m_tag_data.year, m_year_string, sizeof(m_tag_data.year));
}

inline void Mp3Tag::SetComment(char *comment)
{
	strncpy_space(comment, m_tag_data.comment, m_comment_string, sizeof(m_tag_data.comment));
}

#endif // _MP3_TAG_H
