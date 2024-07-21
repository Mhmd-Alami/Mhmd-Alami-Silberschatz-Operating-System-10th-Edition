#README

Project season 3 :

This program is run the codes of terminal in the way that project wants.
It makes line separate by space and make a fork to separate processes :

	if { !! } is the command :
		It will run the previous command that been used.
	if { quit } is the command :
		Program will ends.
	if { | } exists :
		It makes another fork and use pipe to pass data,
		the new child will write the output of arguments before | in pipe,
		the new parent read the new child data from pipe and pass it to arguments after | as an argument.
	else if { > } exists :
		It use the separated line and give them to execvp to be run as in terminal
		and concurrently it will be written in the file that been called after >.
		( this code will show the things that will be written in file. )
	else if { < } exists :
		In this situation a file name would be after <, the file will be read
		after that code will pass it do arguments before < , and it will show the output of it using execvp.
	else :
		This state will use the separated line and pass the line as arguments to execvp and shows the output.
		
