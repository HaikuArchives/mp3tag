/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#include "Mp3Tag.h"

#include <File.h>
#include <stdio.h>
#include <string.h>
#include <Alert.h>
#include <TypeConstants.h>

Mp3Tag::Mp3Tag(entry_ref *file_ref):
	m_file(file_ref,B_READ_WRITE)
{
	if (m_file.InitCheck()!=B_OK)
	{
		m_init = m_file.InitCheck();
		return;
	}

	m_init=B_OK;

	char mime[B_MIME_TYPE_LENGTH];
	m_file.ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, mime, B_MIME_TYPE_LENGTH);
	
	if (strcmp(mime,"audio/x-mpeg") != 0)
		m_init = WRONG_MIME_TYPE;

	m_file.Seek(-128/*-sizeof(tag_struct)*/, SEEK_END);
	m_file.Read(&m_tag_data, 128/*sizeof(tag_struct)*/);
	if (!strncmp(m_tag_data.header, "TAG", 3))
	{
		m_file.Seek(-128/*-sizeof(tag_struct)*/, SEEK_END);
	}
	else
	{
		memset(&m_tag_data, ' ', 128/*sizeof(tag_struct)*/);
		m_tag_data.header[0]='T';
		m_tag_data.header[1]='A';
		m_tag_data.header[2]='G';
		m_tag_data.genre=0;
		m_file.Seek(0, SEEK_END);
	}
	SetTitle(m_tag_data.title);
	SetArtist(m_tag_data.artist);
	SetAlbum(m_tag_data.album);
	SetYear(m_tag_data.year);
	SetComment(m_tag_data.comment);
}

Mp3Tag::~Mp3Tag()
{
}

uint32 Mp3Tag::InitCheck()
{
	return m_init;
}

// the source doesn't have to be NULL-Terminated
// the target_null has to be at least size+1 bytes long
// the target_space has to be at least size bytes long
void Mp3Tag::strncpy_space(char *source,char *target_space,char *target_null,uint8 size)
{
	uint8 cnt = 0;
	while (cnt<size && 0 != source[cnt++]);
	strncpy(target_space, source, size);
	memset(target_space + cnt,' ', size-cnt);
	strncpy(target_null, source, size);
	cnt=size;
	while(' ' == target_null[--cnt] && cnt>0);
	target_null[cnt+1] = 0;
}

void Mp3Tag::WriteToDisk()
{
	m_file.Write(&m_tag_data, sizeof(m_tag_data));
	m_file.Seek(-128/*-sizeof(m_tag_data)*/, SEEK_END);
}
