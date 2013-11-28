/*
	Written by Arvid Norberg 1999 <arvid@iname.com>
*/

#ifndef _MULTI_REQUESTER_WINDOW_H
#define _MULTI_REQUESTER_WINDOW_H

#include "layout.h"
#include "MWindow.h"

class MPopup;
class MButton;
class MTextControl;
class MCheckBox;

const uint32 PATTERN_CHECK='pchk';
const uint32 YEAR_CHECK='ychk';
const uint32 TITLE_CHECK='tchk';
const uint32 ALBUM_CHECK='achk';
const uint32 ARTIST_CHECK='nchk';
const uint32 GENRE_CHECK='gchk';
const uint32 COMMENT_CHECK='cchk';

class MultiRequesterWindow : public MWindow
{
public:
	MultiRequesterWindow(char *string,uint32 size,char *year,char *title,char *artist,char *album,char *comment,int8 *genre);
	virtual void MessageReceived(BMessage *);
	bool Go();
private:
	volatile bool *value;
	MTextControl *pattern_gadget;
	MButton *ok_button;
	
	MCheckBox *year_check;
	MCheckBox *title_check;
	MCheckBox *album_check;
	MCheckBox *artist_check;
	MCheckBox *genre_check;
	MCheckBox *pattern_check;
	MCheckBox *comment_check;

	MTextControl *year_gadget;
	MTextControl *title_gadget;
	MTextControl *album_gadget;
	MTextControl *artist_gadget;
	MTextControl *comment_gadget;
	MPopup *genre_gadget;

	MView *topview;

	char	*pattern_string,
			*year_string,
			*title_string,
			*artist_string,
			*album_string,
			*comment_string;
	int8 *genre_byte;
	int8 tmp_genre;
	uint32 len;

	enum
	{
		OK_BUTTON='okbt',
		CANCEL_BUTTON='clbt'
	};
};

#endif // _MULTI_REQUESTER_WINDOW_H
