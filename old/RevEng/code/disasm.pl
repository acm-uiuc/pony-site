#!/usr/bin/perl -w

# Disasm.pl v0.4

#Assumes that the file we're working with is stripped
#TODO: 
# 0. Sort NUMERICALLY on function call names, not lexographically
# 1. Get this to work if symbols are present
# 2. Add options or speed up the finding of unused functions (can we do it
#    without an extra pass?)
# 3. Fix various FIXME's
# 4. Make work with sparc

use strict;
use Getopt::Long;

my ($call_graph, $fnames);

if($#ARGV < 0)
{
    print "Usage: $0 <file> [options]\n";
	print "\t--fnames\tprint function names\n";
	print "\t--graph\tgenerate file with graph information for dot\n";
    exit 1;
} else {
	
	$call_graph = $fnames = 0;
	GetOptions( "fnames"    => \$fnames,     # --fnames
				"graph"    => \$call_graph   # --graph
				);
	
}



my %symbols;

my $fprefix = "function #";
my $lprefix = "label #";

my $return   = "ret    ";
my $call     = "call   0x";
my $jump     = "j..    0x";
my $retsize = 1; #size of ret opcode

#Sparc:
#FIXME: There's a few issues with sparc opcodes:
# 1. We need to handle command line options to specify to use sparc
# 2. We need to allow arbitrary spacing after the branch instruction
# 3. Some functions return with just ret, some return with ret then restore
my $sreturn  = "restore";
my $scall    = "call  ";
my $sjump    = "  b[^t0-9].[^O-9a-f].[ ]*";
my $sparc    = `uname -a` =~ /sparc/;

if($sparc)
{
   $return = $sreturn;
   $call = $scall;
   $jump = $sjump;
}

my $filename = shift(@ARGV);
my @lines = `objdump -TC $filename`;

my %functions;
my %labels;
my %calls;



foreach (@lines)
{
    if(/0([0-9a-f]+).*\*UND\*.* ([^ ]+)$/)
    {
        my $temp = $2;
        chop $temp;
        $symbols{$1} = $temp;
    }
}

@lines = `objdump -dj .text $filename`;


#counters for functions, unused functions, and labels
my $fcount = 1;
my $lcount = 1;

foreach(@lines)
{
    #FIXME: Hack that also assumes stripped binary.. How can we factor this
    #out of the loop?
    if(/([0]+)([0-9a-f]+)\ <.text>/)
    {
   #     print "Text @ $1 $2 ($_)";
        $symbols{$2} = "_start";    
        $functions{$2} = "_start";
    }
    
    if(/$call([0-9a-f]+)/ &&
        ! exists($symbols{$1}))
   # if(/([0-9a-f]+).*$function/)
    {
        $symbols{$1} = "$fprefix$fcount";
        $fcount++;

	$functions{$1} = $symbols{$1};
    }
    elsif(/$jump([0-9a-f]+)/ &&
          ! exists($symbols{$1}))
    {
        $symbols{$1} = "$lprefix$lcount";
        $lcount++;

	$labels{$1} = $symbols{$1};
    }
}

#FIXME: This should be an option...
#
# Nasko - should it? misses some data in the output just uncomment the if
# statement and the corresponding closing brace to make --fnames work
#

my $inFunc = 0;
my $lastRet;
my $storeRet = 0;
my $counter = 0;


# if ($fnames == 1) {

	foreach(@lines)
	{
		++$counter;
	    #HACK: Yeah, this sucks.. but we can't just add 1 to get the next address
	    #and I don't know how to peek at the next line
		#
		# Nasko - just use $lines[$counter]
		#
#	    if($storeRet == 1)
#	    {
#	        if(/([0-9a-f]+)/)
#	        {
#	            $lastRet = $1;
#	            $storeRet = 0;
#	        }
#	        else
#	        {
#	            next;
#	        }
#	    }
	    if(/([0-9a-f]+)/ and exists($functions{$1}))
	    {
	        $inFunc = 1;
	    }
	    elsif(/([0-9a-f]+).*$return/)
	    {
	    	 
			$lastRet = 1;
		    if($inFunc == 0)
	        {
	           $symbols{$lastRet} = "function #$fcount (unused)";
	           $functions{$lastRet} = "function #$fcount (unused)";
	           $fcount++;
	        }
	        #FIXME: Sure would be nice to peek at the next line and set lastRet
	        #right here..
	        $storeRet = 1;
	        $inFunc = 0;
				
	   }
	}
#}

my $localFunc;
foreach(@lines)
{

    if(/([0-9a-f]+)/ and exists($symbols{$1}))
    {
    	my $symb = $symbols{$1};
    	if ($symb =~ /label/) {
            $_ = "$symb:\n$_";
        } else {
	    $_ = "\n$symb:\n$_";
	    $localFunc = $symb;
	}
    }
    elsif(/.*$return/)
    {
        $_ .= "return\n\n";
    }
    elsif(/.*$jump([0-9a-f]+)/ ||
          /.*$call([0-9a-f]+)/)
    {
        chop;
        if(exists($symbols{$1}))
        {
            $_ .= "\t<" . $symbols{$1} . ">\n";
        }
        else
        {
            $_ .= "\t<unknown symbol>\n";
        }
	
	my $symb = $symbols{$1};

# Why skip labels??
#	if ($symb =~ /label/) {
#	    next;
#	}
		
	if(exists($calls{ $localFunc }))
        {
            push @{$calls{ $localFunc } }, $symb;
        }
        else
        {
            $calls{ $localFunc } = [ $symb ];
        }
    }
    print;
}

print "\nKnown symbols:\n";

foreach (sort (keys %symbols))
{
    if (!($symbols{$_} =~ /label/)) 
    {
        print;
        print ": $symbols{$_}\n";
    }
}

print "\nCall graph:\n";

my $fName;
if ($call_graph == 1) {
	# a local variable for each function name
	# open the file to store the definition of the graph
	open(FILE, ">call_graph") || 
		die "Couldn't open file for writing the call graph\n";
	print FILE "digraph prof {\n";
}

#foreach (sort keys %calls)
foreach (%calls)
{
	$fName = $_;
	
    print;
    print ":\n";
    foreach (@{ $calls{$_} })
    {
	my $mytmp = $_;
        if(!($_ =~ /label/))
        {
                print " calls $_\n";
				
				# print to the graph file
                if($call_graph == 1) {print FILE "\t\"$fName\" -> \"$_\"\n"};
        }
    }
    print "\n";
}

if ($call_graph == 1) {
	# put the closing brace and close the file
	print FILE "}\n";
	close(FILE);
}
