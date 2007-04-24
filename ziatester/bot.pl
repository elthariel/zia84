#!/usr/bin/perl -Tw
## bot.pl for Zi@T5sTER in /home/lessyv/taf/zia/ziatester
## 
## made by Christophe Malinge
##         <maling_c@lse.epita.fr>
## 
## started on    Mon Apr 23 11:21:09 2007   lessyv
## last update   Wed Apr 25 01:51:39 2007   loic
##

use strict;
BEGIN { $ENV{PATH} = '/usr/ucb:/bin' }
use Socket;
use Carp;
my $EOL = "\015\012";

sub spawn;  # forward declaration
sub reply;  # ' '
sub logmsg { print "$0 $$: @_ at ", scalar localtime, "\n" }

my $port = shift || 31337;
my $proto = getprotobyname('tcp');

($port) = $port =~ /^(\d+)$/                        or die "invalid port";

socket(Server, PF_INET, SOCK_STREAM, $proto)	|| die "socket: $!";
setsockopt(Server, SOL_SOCKET, SO_REUSEADDR,
	   pack("l", 1)) 	|| die "setsockopt: $!";
bind(Server, sockaddr_in($port, INADDR_ANY))	|| die "bind: $!";
listen(Server,SOMAXCONN) 				|| die "listen: $!";

print "[ ok ] server started on port $port\n";

my $waitedpid = 0;
my $paddr;

use POSIX ":sys_wait_h";
sub REAPER {
    my $child;
    while (($waitedpid = waitpid(-1,WNOHANG)) > 0) {
	print "[DISCONNECT] $waitedpid" . ($? ? " with exit $?" : '') . "\n\n";
    }
    $SIG{CHLD} = \&REAPER;  # loathe sysV
}
my $computed = 0;

$SIG{CHLD} = \&REAPER;

my $client_closed = 0;

for ( $waitedpid = 0;
      ($paddr = accept(Client,Server)) || $waitedpid || !($client_closed);
      $waitedpid = 0, close Client)
{
    next if $waitedpid and not $paddr;
    my($port,$iaddr) = sockaddr_in($paddr);
    my $name = gethostbyaddr($iaddr,AF_INET);
    
    $computed++;
    print "[CONNECT] from $name [",
    inet_ntoa($iaddr), "]:$port\n";

    spawn sub
    {
	my $test;
	my $request_index;
	my $request_string;
	my $input;
	my $query_sent = 0;
	
	$|=1;
	$test = 0;
	$request_string = 'tachate';
	$request_index = 0;
	my $filename = "request_" . $computed;
	my $check_write = !(-r $filename);
	
	if ($check_write)
	{
	    open(INPUT, '>>', $filename)
		or die("Can't open file $filename : $!\n");
	}

	while (<Client>)
	{
	    my $uuu;
	    my $len = length($_);

	    $uuu = $_;
	    $uuu =~ s/\r//g;
	    $uuu =~ s/\n//g;
	    print "[request $len] $uuu\n";
	    if ($check_write)
	    {
		syswrite(INPUT, $_);
	    }
	    if ((index($_, "\r\n", 1) == -1) && !($query_sent))
	    {
		my $output = 'tachate';

		$request_string = reply($request_index);
#		if ($computed == 25)
# 		       {
# 		        	close(Client);
			       
# 		       }
# 		else
# 		{
		    send(Client, $request_string, 0);
# 		}
		$output = $request_string;
		$output =~ s/\r//g;
		$output =~ s/\n//g;
		print "[REPLYING $computed] $output\n";
		$query_sent = 1;
# XX test anti KOOO    if ($computed == 19)
# 		       {
# 		        	close(Client);
# 		       }
	    }
		#send(Client, "HTTP/1.1 200 OK\r\n\r\n", 0);
		$request_index = 0;
	}
	if ($check_write)
	{
	    close(INPUT);
	}
    }
};

sub spawn {
    my $coderef = shift;

    unless (@_ == 0 && $coderef && ref($coderef) eq 'CODE') {
	confess "usage: spawn CODEREF";
    }
    
    my $pid;
	if (!defined($pid = fork)) {
	    print "cannot fork: $!\n";
	    return;
	} elsif ($pid) {
	    #print "[fork] pid:$pid\n";
	    return; # I'm the parent
	}
    # else I'm the child -- go spawn
    
    #open(STDIN,  "<&Client")   || die "can't dup client to stdin";
    #open(STDOUT, ">&Client")   || die "can't dup client to stdout";
    ## open(STDERR, ">&STDOUT") || die "can't dup stdout to stderr";
    exit &$coderef();
}

sub reply
{
    #print "ok cheating... , query is ", $computed, " ";
    if ($computed == 1)
    {
	# OK HTTP/1.1 200 Ok
	return "HTTP/1.1 302 Found\r\n\r\n";
    }
    if ($computed == 2)
    {
	return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 3)
    {
	return "HTTP/1.1 200 OK\r\n\r\n";
	# le biaiseuuu return "HTTP/1.1 404 Not Found";
    }
    if ($computed == 4)
    {
	return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 5)
    {
	return "HTTP/1.1 302 Found\r\n\r\n";
# KO avec 200
    }
    if ($computed == 6)
    {
	return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 7)
    {
 	return "HTTP/1.1 200 OK\r\n\r\n";
 	#return "HTTP/1.1 302 Found\r\n\r\n";
# KO avec 404
# KO avec 302
    }
    if ($computed == 8)
    {
	return "HTTP/1.1 400 Bad Request\r\n\r\n";
# KO avec 404
    }
    if ($computed == 9)
    {
 	return "HTTP/1.1 200 OK\r\n\r\n";
# KO avec 302
    }
    if ($computed == 10)
    {
  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 11)
    {
 	return "HTTP/1.1 200 OK\r\n\r\n";
    }
    if ($computed == 12)
    {
 	return "HTTP/1.1 400 Bad Request\r\n\r\n";
#200 OK avant hier ...;
# KO avec 302
    }
    if ($computed == 13)
    {
 	return "HTTP/1.1 200 OK\r\n\r\n";
    }
    if ($computed == 14)
    {
  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 15)
    {
  	return "HTTP/1.1 200 OK\r\n\r\n";
#Allow: GET, HEAD, OPTIONS, TRACE
    }
    if ($computed == 16)
    {
  	return "HTTP/1.1 200 OK\r\n\r\n";
#OK avant hier ... HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 17)
    {
#  	return "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET, HEAD, OPTIONS, TRACE\r\n\r\n";
 # 	return "HTTP/1.1 400 Bad Request\r\n\r\n";
# 	return "HTTP/1.1 411 Length Required\r\n\r\n";
#	return "HTTP/1.1 302 Found\r\n\r\n";
	return "HTTP/1.1 200 OK\r\n\r\n";
    }
    if ($computed == 18)
    {
  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
#return "HTTP/1.1 403 Forbidden\r\n\r\n";
# KO avec 404
    }
    if ($computed == 19)
    {
  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
# return "HTTP/1.1 403 Forbidden\r\n\r\n"; # XX KOOOO
# KO avec 404 et ses potes
# KO avec 300 et 302
    }
    if ($computed == 20) #ErrorGet06
    {
  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
#  	return "HTTP/1.1 200 OK";
    }
    if ($computed == 21)
    {
#  	return "HTTP/1.1 400 Bad Request\r\n\r\n";
	return "HTTP/1.1 411 Length Required\r\n\r\n";
#	return "HTTP/1.1 302 Found\r\n\r\n";
#	return "HTTP/1.1 200 OK\r\n\r\n";
#  	return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
#HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 22)
    {
  	return "HTTP/1.1 403 Forbidden\r\n\r\n";
#HTTP/1.1 400 Bad Request\r\n\r\n";
    }
    if ($computed == 23)
    {
	return "HTTP/1.1 200 OK\r\n\r\n";
#	return "HTTP/1.1 302 Found\r\n\r\n";
    }

    if ($computed == 24)
    {
	return "HTTP/1.1 200 OK\r\n\r\n";
    }
    if ($computed == 25)
    {

#  	return "HTTP/1.1 403 Forbidden\r\n\r\n";
	return "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n";
#	return "501 Not Implemented\r\n\r\n";
#	return "HTTP/1.1 400 Bad Request\r\n\r\n";
#	return "HTTP/0.9 300 Multiple Choices\r\n\r\n";
#	return "HTTP/0.9 200 OK\r\n\r\n"; #KO free
#	return "HTTP/0.9 302 Found\r\n\r\n"; #KO linuxfr
# google cloture direct
    }
    if ($computed == 26)
    {
#  	return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
#	return "HTTP/1.1 200 OK\r\n\r\n";
	return "HTTP/1.1 400 Bad Request\r\n\r\n";
#	return "HTTP/1.1 302 Found\r\n\r\n";
    }

    return "ChiIIiIiicHEEEE\r\n\r\n";
}
