var lock = false;

function setSelected(el, value)
{
	if(!lock)
	{
		lock = true;
		
		$(el).attr('class', 'buttonSelected');
		globalView.transfertString(value);
	}
}