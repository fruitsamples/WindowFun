About WindowFun

This sample demonstrates how to:

	� Create an arbitrary number of window layers within your application.  Your application
		will typically create windows into one of the predefined classes, floating, utility, etc., 
		but with Mac OS X you have the ability to create an arbitrary number of window groups
		and layers, as well as set attributes to those groups.
		
	� Display an overlay window which is attached to the main window.  Creating an overlay window 
		that moves with its parent may be the easiest way to draw transparent text on a movie,
		for instance.
	
	� A method of drawing transparent lines by drawing to an overlay window. In the past it was 
		a comon technique to XOR the bits on your window to draw overlay lines or show selection.
		With Mac OS X you may chose to draw semi-transparently into an overlay window for a similar
		effect.
	
	� How to display the "Poof" the toolbar uses, similar to the dock "Poof".
