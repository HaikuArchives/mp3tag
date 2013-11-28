/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#ifndef _REQUESTER_WINDOW_H
#define _REQUESTER_WINDOW_H

#include "layout.h"
#include "MWindow.h"

class MButton;
class MTextControl;
class MCheckBox;
class MPopup;

class RequesterWindow : public MWindow
{
public:
	RequesterWindow(char *wintitle,char *year,char *title,char *artist,char *album,char *comment,int8 *genre);
	virtual void MessageReceived(BMessage *);
	bool Go();
private:
	volatile bool *value;
	MButton *ok_button;
	MTextControl *year_gadget;
	MTextControl *title_gadget;
	MTextControl *album_gadget;
	MTextControl *artist_gadget;
	MTextControl *comment_gadget;
	MPopup *genre_gadget;
	MView *topview;

	char	*year_string,
			*title_string,
			*artist_string,
			*album_string,
			*comment_string;
	int8 *genre_byte;

	enum
	{
		OK_BUTTON='okbt',
		CANCEL_BUTTON='clbt'
	};
};

#endif // _REQUESTER_WINDOW_H