/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#include "RequesterWindow.h"
#include "MultiRequesterWindow.h"
#include "Mp3Tag.h"
#include <string.h>
#include <stdio.h>
#include <Message.h>
#include <Alert.h>
#include <Entry.h>
#include <Path.h>
#include <TrackerAddOn.h>

_EXPORT void process_refs(entry_ref ref,BMessage *msg,void *);

bool match(char *name,uint8 name_offs,char *pattern,uint8 pattern_offs,char *title,char *artist,char *album,char *year,char *comment);

// recursive pattern matcher
bool match(char *name,uint8 name_offs,char *pattern,uint8 pattern_offs,char *title,char *artist,char *album,char *year,char *comment)
{
	char dummy;
	char *target;
	uint8 max_len; // the length of the target string
	bool result=false;
	while(pattern_offs<strlen(pattern) && name_offs<strlen(name))
	{
		if (pattern[pattern_offs]=='/')
		{
			uint8 target_offs=0;
			target=0;
			switch(pattern[++pattern_offs])
			{
			case 'n':
				target=album;
				max_len=30;
				break;
			case 'a':
				target=artist;
				max_len=30;
				break;
			case 't':
				target=title;
				max_len=30;
				break;
			case 'y':
				target=year;
				max_len=4;
				break;
			case 'c':
				target=comment;
				max_len=30;
				break;
			case '*':
				target=&dummy;
				max_len=0;
				break;
			default:
				return false;
				break;
			}
			pattern_offs++;
			while (name_offs<strlen(name) && !(result=match(name,name_offs,pattern,pattern_offs,title,artist,album,year,comment)) )
			{
				if (target_offs<max_len) target[target_offs++]=name[name_offs];
				name_offs++;
			}
			target[target_offs]=0;
			return (result || (pattern[pattern_offs]==0 && name[name_offs]==0));
		}
		if (name[name_offs]!=pattern[pattern_offs]) break;
		pattern_offs++;
		name_offs++;
	}
	return (pattern[pattern_offs]==0 && name[name_offs]==0);
}


void process_refs(entry_ref ref,BMessage *msg,void *)
{
	BPath path;
	entry_ref file_ref;
	char year[5],artist[31],album[31],title[31],comment[31];
	int8 genre;

	int32 file_count;
	type_code num_types;

	msg->GetInfo("refs",&num_types,&file_count);
	if (file_count>1) // ************************ MULTIFILE *********************
	{
		char filename[B_FILE_NAME_LENGTH];
		uint32 cnt=0;
		char pattern[200];
		*pattern=-1;
		char artist_tmp[31],album_tmp[31],title_tmp[31],year_tmp[5],comment_tmp[31];

		genre=-1;
		*year=-1;
		*artist=-1;
		*album=-1;
		*title=-1;
		*comment=-1;

		MultiRequesterWindow *window=new MultiRequesterWindow(pattern,200,year,title,artist,album,comment,&genre);
		if (!window->Go())
			return;

		enum
		{
			PROMPT_MODE = 0,
			CHANGE_ALL,	// change the mime type of all files
			PROCESS_ALL, // don't change the mime type of all files, but process them anyway
		} mode = PROMPT_MODE;

		while (B_NO_ERROR == msg->FindRef("refs",cnt++,&file_ref))
		{
			Mp3Tag tag(&file_ref);
			uint32 error_code;

			error_code = tag.InitCheck();

			char error_string[120+B_FILE_NAME_LENGTH];
			char name[B_FILE_NAME_LENGTH];
				
			BEntry entry(&file_ref);
			entry.GetName(name);

			if (error_code == WRONG_MIME_TYPE)
			{
				if (mode == CHANGE_ALL)
				{
					tag.SetMime();
				}
				else if (mode == PROMPT_MODE)
				{
					sprintf(error_string,"\"%s\" has the wrong MIME-type, an MPEG-file should have the MIME-Type \"audio/x-mpeg\". Do you want to change it's MIME-type?",name);
					BAlert *alert=new BAlert("Error",error_string,"Yes","Change all","No");
					int ret = alert->Go();
				
					switch (ret)
					{
						case 0:
							tag.SetMime();
							break;
						case 1:
							tag.SetMime();
							mode = CHANGE_ALL;
							break;
						case 2:
							ret = (new BAlert("Error","Do you want to process all files without changing their MIME-type?","Yes (I'm sure every file is an mp3 file)","No (just this file)","No (Cancel everything)"))->Go();
							switch (ret)
							{
								case 0:
									mode = PROCESS_ALL;
									break;
								case 2:
									return;
							}
							break;
					}
				}
				error_code = B_OK;
			}

			if (error_code != B_OK )
			{
				switch (error_code)
				{
				case B_NO_MEMORY:
					sprintf(error_string,"Out of memory when opening \"%s\"",name);
					break;
				case B_PERMISSION_DENIED:
					sprintf(error_string,"\"%s\" Permission denied",name);
					break;
				case B_ENTRY_NOT_FOUND:
					sprintf(error_string,"\"%s\" not found",name);
					break;
				default:
					sprintf(error_string,"Some error on the file \"%s\"",name);
					break;
				}
			BAlert *alert=new BAlert("Error opening file",error_string,"ok");
			alert->Go();
			continue;
		}

			if (*pattern!=-1)
			{
				BEntry entry(&file_ref);
				entry.GetName(filename);

				*title_tmp=-1;
				*album_tmp=-1;
				*artist_tmp=-1;
				*comment_tmp=-1;
				*year_tmp=-1;

				// pattern matching
				if (match(filename,0,pattern,0,title_tmp,artist_tmp,album_tmp,year_tmp,comment_tmp))
				{
					if (*artist_tmp!=-1) tag.SetArtist(artist_tmp);
					if (*album_tmp!=-1) tag.SetAlbum(album_tmp);
					if (*title_tmp!=-1) tag.SetTitle(title_tmp);
					if (*year_tmp!=-1) tag.SetYear(year_tmp);
					if (*comment_tmp!=-1) tag.SetComment(comment_tmp);
				}
				else
				{
					char error_string[60+B_FILE_NAME_LENGTH];
					sprintf(error_string,"\"%s\" did not match the pattern and is left untouched",filename);
					BAlert *alert=new BAlert("Pattern",error_string,"ok");
					alert->Go();
				}
			}

			if (genre!=-1) tag.SetGenre(genre);
			if (*year!=-1) tag.SetYear(year);
			if (*artist!=-1) tag.SetArtist(artist);
			if (*album!=-1) tag.SetAlbum(album);
			if (*title!=-1) tag.SetTitle(title);
			if (*comment!=-1) tag.SetComment(comment);
			tag.WriteToDisk();
		}
	}
	else // ************************ SINGLEFILE *********************
	{
		if (B_NO_ERROR!=msg->FindRef("refs",&file_ref)) return;

		Mp3Tag tag(&file_ref);

		uint32 error_code;
		error_code = tag.InitCheck();

		char *error_string;
				
		if (error_code == WRONG_MIME_TYPE)
		{
			error_string = "the file has the wrong MIME-type, an MPEG-file should have the MIME-Type \"audio/x-mpeg\". Do you want to change it's MIME-type?";
			BAlert *alert=new BAlert("Error",error_string,"Yes","No (but process it anyway)","No (cancel)");
			int ret = alert->Go();
		
			switch (ret)
			{
				case 0:
					tag.SetMime();
					break;
				case 2:
					return;
			}
			error_code = B_OK;
		}

		if (error_code != B_OK)
		{
			switch (error_code)
			{
			case B_NO_MEMORY:
				error_string="Out of memory";
				break;
			case B_PERMISSION_DENIED:
				error_string="File permission denied";
				break;
			case B_ENTRY_NOT_FOUND:
				error_string="File not found";
				break;
			default:
				error_string="error";
				break;
			}
			BAlert *alert=new BAlert("Error opening file",error_string,"ok");
			alert->Go();
			return;
		}

		char filename[B_FILE_NAME_LENGTH];
		BEntry entry(&file_ref);
		entry.GetName(filename);

		strcpy(year,tag.Year());
		strcpy(title,tag.Title());
		strcpy(artist,tag.Artist());
		strcpy(album,tag.Album());
		strcpy(comment,tag.Comment());
		genre=tag.Genre();

		RequesterWindow *window=new RequesterWindow(filename,year,title,artist,album,comment,&genre);
		if (!window->Go())
			return;
		tag.SetTitle(title);
		tag.SetArtist(artist);
		tag.SetAlbum(album);
		tag.SetYear(year);
		tag.SetComment(comment);
		tag.SetGenre(genre);
		tag.WriteToDisk();
	}
}
