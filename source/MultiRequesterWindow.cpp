/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#include "VGroup.h"
#include "HGroup.h"
#include "MButton.h"
#include "MBorder.h"
#include "MCheckBox.h"
#include "MStringView.h"
#include "MTextControl.h"
#include "MWindow.h"
#include "layout.h"
#include "MPopup.h"

#include "MultiRequesterWindow.h"

#include <string.h>
#include <stdio.h>
#include <OS.h>

MultiRequesterWindow::MultiRequesterWindow(char *pattern,uint32 size,char *year,char *title,char *artist,char *album,char *comment,int8 *genre):
	MWindow(BRect(100,100,180,150),"mp3tag [multiple files]",B_TITLED_WINDOW,B_NOT_RESIZABLE),
	pattern_string(pattern),
	year_string(year),
	title_string(title),
	artist_string(artist),
	album_string(album),
	comment_string(comment),
	genre_byte(genre),
	len(size)
{
	topview=
	new VGroup
	(
		new MBorder(M_LABELED_BORDER,10,"pattern",
			new VGroup
			(
				new MStringView("Enter the patter by which your mp3 files are named."),
				new MStringView("/a = Artist name (30 chars)"),
				new MStringView("/t = song Title (30 chars)"),
				new MStringView("/n = album Name (30 chars)"),
				new MStringView("/c = comment (30 chars)"),
				new MStringView("/y = year (4 chars)"),
				new MStringView("/* = wildcard, can be anything"),
				pattern_check=new MCheckBox("use pattern matching",new BMessage(PATTERN_CHECK)),
				pattern_gadget=new MTextControl("pattern:","/a - /t.mp3"),
				0
			)
		),
		new MBorder(M_LABELED_BORDER,10,"set every tag to:",
			new HGroup
			(
				new VGroup
				(
					minimax(-1,-1,-1,-1),
					title_check=new MCheckBox("Title:",new BMessage(TITLE_CHECK)),
					artist_check=new MCheckBox("Artist:",new BMessage(ARTIST_CHECK)),
					album_check=new MCheckBox("Album:",new BMessage(ALBUM_CHECK)),
					comment_check=new MCheckBox("Comment:",new BMessage(COMMENT_CHECK)),
					year_check=new MCheckBox("Year:",new BMessage(YEAR_CHECK)),
					genre_check=new MCheckBox("Genre:",new BMessage(GENRE_CHECK)),
					0
				),
				new VGroup
				(
					title_gadget=new MTextControl("",""),
					artist_gadget=new MTextControl("",""),
					album_gadget=new MTextControl("",""),
					comment_gadget=new MTextControl("",""),
					year_gadget=new MTextControl("",""),
					genre_gadget=new MPopup("select",
	"Blues","Classic Rock","Country","Dance","Disco","Funk","Grunge","Hip-Hop","Jazz","Metal",
	"New Age","Oldies","Other","Pop","R&B","Rap","Reggae","Rock","Techno","Industrial","Alternative","Ska","Death Metal",
	"Pranks","Soundtrack","Euro-Techno","Ambient","Trip-Hop","Vocal","Jazz+Funk","Fusion",
	"Trance","Classical","Instrumental","Acid","House","Game","Sound Clip","Gospel",
	"Noise","AlternRock","Bass","Soul","Punk","Space","Meditative","Instrumental Pop",
	"Instrumental Rock","Ethnic","Gothic","Darkwave","Techno-Industrial","Electronic",
	"Pop-Folk","Eurodance","Dream","Southern Rock","Comedy","Cult","Gangsta","Top 40",
	"Christian Rap","Pop/Funk","Jungle","Native American","Cabaret","New Wave","Psychadelic",
	"Rave","Showtunes","Trailer","Lo-Fi","Tribal","Acid Punk","Acid Jazz","Polka",
	"Retro","Musical","Rock & Roll","Hard Rock", "Folk","Folk/Rock","National Folk","Swing",
	"Fusion","Bebob","Latin","Revival","Celtic","Bluegrass","Avantgarde","Gothic Rock",
	"Progress. Rock","Psychadel. Rock","Symphonic Rock","Slow Rock","Big Band","Chorus",
	"Easy Listening","Acoustic","Humour","Speech","Chanson","Opera","Chamber Music",
	"Sonata","Symphony","Booty Bass","Primus","Porn Groove","Satire",	"Unknown",0),
					0
				),
				0
			)
		),
		new HGroup
		(
			new MButton("Cancel",new BMessage(CANCEL_BUTTON)),
			ok_button=new MButton("Run",new BMessage(OK_BUTTON)),
			0
		),
		0
	);
	AddChild(dynamic_cast<BView*>(topview));
	SetDefaultButton(ok_button);
	year_gadget->SetEnabled(false);
	title_gadget->SetEnabled(false);
	artist_gadget->SetEnabled(false);
	album_gadget->SetEnabled(false);
	comment_gadget->SetEnabled(false);
	pattern_gadget->SetEnabled(false);

	genre_gadget->SetActive(0);
	genre_gadget->SetEnabled(false);
}

bool MultiRequesterWindow::Go()
{
	bool result;
	value = &result;
	// show the window
	Show();
//	acquire_sem(wait);
	int32 ret_val;
	wait_for_thread(Thread(), &ret_val);
	return result;
}

void MultiRequesterWindow::MessageReceived(BMessage *message)
{
	int32 val;
	switch (message->what)
	{
		case OK_BUTTON:
			*value=true;

			// copy the data
			if (pattern_check->Value())
				strncpy(pattern_string,pattern_gadget->Text(),len);
			if (year_check->Value())
				strncpy(year_string,year_gadget->Text(),4);
			if (title_check->Value())
				strncpy(title_string,title_gadget->Text(),30);
			if (artist_check->Value())
				strncpy(artist_string,artist_gadget->Text(),30);
			if (album_check->Value())
				strncpy(album_string,album_gadget->Text(),30);
			if (comment_check->Value())
				strncpy(comment_string,comment_gadget->Text(),30);
			if (genre_check->Value())
				*genre_byte=tmp_genre;

			// lock the window (prepare to quit)
			Lock();
			// quit
			Quit();
			break;
		case B_QUIT_REQUESTED:
		case CANCEL_BUTTON:
			*value=false;
			// lock the window (prepare to quit)
			Lock();
			// quit
			Quit();
			break;
		case YEAR_CHECK:
			message->FindInt32("be:value",&val);
			year_gadget->SetEnabled(val);
			break;
		case TITLE_CHECK:
			message->FindInt32("be:value",&val);
			title_gadget->SetEnabled(val);
			break;
		case ARTIST_CHECK:
			message->FindInt32("be:value",&val);
			artist_gadget->SetEnabled(val);
			break;
		case ALBUM_CHECK:
			message->FindInt32("be:value",&val);
			album_gadget->SetEnabled(val);
			break;
		case COMMENT_CHECK:
			message->FindInt32("be:value",&val);
			comment_gadget->SetEnabled(val);
			break;
		case GENRE_CHECK:
			message->FindInt32("be:value",&val);
			genre_gadget->SetEnabled(val);
			break;
		case PATTERN_CHECK:
			message->FindInt32("be:value",&val);
			pattern_gadget->SetEnabled(val);
			break;
		case '!pop':
    	uint32 index;
    	if (B_NO_ERROR==message->FindInt32("index",(int32*)&index))
    		tmp_genre=index;
			break;
		default:
			MWindow::MessageReceived(message);
	}
}
