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
#include "MPopup.h"
#include "MWindow.h"
#include "layout.h"

#include "RequesterWindow.h"

#include <string.h>
#include <stdio.h>
#include <OS.h>

RequesterWindow::RequesterWindow(char *filename,char *year,char *title,char *artist,char *album,char *comment,int8 *genre):
	MWindow(BRect(100,100,101,101),"mp3-tag editor",B_TITLED_WINDOW,B_NOT_RESIZABLE),
	year_string(year),
	title_string(title),
	artist_string(artist),
	album_string(album),
	comment_string(comment),
	genre_byte(genre)
{
	topview=
	new VGroup
	(
		new HGroup
		(
			new VGroup
			(
				minimax(-1,-1,-1,-1),
				new MStringView("filename:",B_ALIGN_RIGHT),
				new MStringView("title[30]:",B_ALIGN_RIGHT),
				new MStringView("artist[30]:",B_ALIGN_RIGHT),
				new MStringView("album[30]:",B_ALIGN_RIGHT),
				new MStringView("comment[30]:",B_ALIGN_RIGHT),
				new MStringView("year[4]:",B_ALIGN_RIGHT),
				0
			),
			new VGroup
			(
				minimax(200,-1,-1,-1),
				new MStringView(filename),
				title_gadget=new MTextControl("",title),
				artist_gadget=new MTextControl("",artist),
				album_gadget=new MTextControl("",album),
				comment_gadget=new MTextControl("",comment),
				new HGroup
				(
					year_gadget=new MTextControl("",year),
					genre_gadget=new MPopup("Genre:",
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
			),
			0
		),
		new HGroup
		(
			new MButton("Cancel",new BMessage(CANCEL_BUTTON)),
			ok_button=new MButton("Save",new BMessage(OK_BUTTON)),
			0
		),
		0
	);
	AddChild(dynamic_cast<BView*>(topview));
	SetDefaultButton(dynamic_cast<BButton*>(ok_button));
	genre_gadget->SetActive(*genre);
}

bool RequesterWindow::Go()
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

void RequesterWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case OK_BUTTON:
			*value=true;

			// copy the data
			strncpy(year_string,year_gadget->Text(),4);
			strncpy(title_string,title_gadget->Text(),30);
			strncpy(artist_string,artist_gadget->Text(),30);
			strncpy(album_string,album_gadget->Text(),30);
			strncpy(comment_string,comment_gadget->Text(),30);

			// lock the window (prepare to quit)
			Lock();
			// wait for that thread to copy the return value before we delete the window object
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
    case '!pop':
    	uint32 index;
    	if (B_NO_ERROR==message->FindInt32("index",(int32*)&index))
    		*genre_byte=index;
    	break;
		default:
			MWindow::MessageReceived(message);
	}
}