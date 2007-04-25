#!/usr/bin/perl
print 
"<head>
	<title>CGI exemple </title>
</head>
<body>

<hr>
<h1> Text area example</h1>
<hr>
<p>
Vous avez envoyer ce message :
<blockquote>
";

%incoming = &read_input;
$your_text = $incoming{'review'}; 
print $your_text;

print "</blockquote>
Merci.

<br>
<br>
<br>
</body>
";


sub read_input
{
    
    local ($buffer, @pairs, $pair, $name, $value, %FORM);
    
    $ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;
    if ($ENV{'REQUEST_METHOD'} eq "POST")
    {
	read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
    } else
    {
	$buffer = $ENV{'QUERY_STRING'};
    }
    
    @pairs = split(/&/, $buffer);
    foreach $pair (@pairs)
    {
	($name, $value) = split(/=/, $pair);
	$value =~ tr/+/ /;
	$value =~ s/%(..)/pack("C", hex($1))/eg;
	$FORM{$name} = $value;
    }
    %FORM;
}
