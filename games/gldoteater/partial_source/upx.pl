#use strict;
use warnings;

# I want to run upx on the file gldoteater.exe
# this will produce 2 files named
# gldoteaterb.exe
# gldoteaterd.exe
# if those files are not produced assume the program is already compressed and exit
# I want to find the filesize of each of these files
# delete all but the smallest
# then rename the smallest gldoteater.exe

$orignal_file     = "gldoteater.exe";
$compressed_fileb = "gldoteaterb.exe";
$compressed_filed = "gldoteaterd.exe";
$upx_path         = "C:\\Documents and Settings\\Default\\Desktop\\utils\\upx125w\\upx.exe";
$bWaitForKeyPressBeforeExit = 0;

if( !(-e $orignal_file) )
{
   print "No file to compress.\n";
   if( $bWaitForKeyPressBeforeExit == 1 )
   {
      <>;
   }
   exit;
}

@filestats = stat( $orignal_file );
$orignalfilesize = $filestats[7]; # gldoteater.exe
print "Orignal size = $orignalfilesize\n";

print "Compressing file with method b\n";
@args = ( $upx_path, "--best", "--crp-ms=999999", "--compress-icons=0", "--compress-exports=0", "--nrv2b", "-o", $compressed_fileb, $orignal_file  );
system( @args );
print "Compressing file with method d\n";
@args = ( $upx_path, "--best", "--crp-ms=999999", "--compress-icons=0", "--compress-exports=0", "--nrv2d", "-o", $compressed_filed, $orignal_file );
system( @args );

if( !(-e $compressed_fileb) || !(-e $compressed_filed) )
{
   print "File appears to be already compressed.\n";
   if( $bWaitForKeyPressBeforeExit == 1 )
   {
      <>;
   }
   exit;
}

@filestats = stat( $compressed_fileb );
$filesizeb       = $filestats[7]; # gldoteaterb.exe
print "$compressed_fileb size $filesizeb\n";
@filestats = stat( $compressed_filed );
$filesized       = $filestats[7]; # gldoteaterd.exe
print "$compressed_filed size $filesized\n";

if( $filesizeb < $filesized )
{
   unlink $compressed_filed;
   unlink $orignal_file;
   rename( $compressed_fileb, $orignal_file );
   print "$compressed_fileb appears to be smaller\n";
   print "Renaming smallest file.\n";
   if( $bWaitForKeyPressBeforeExit == 1 )
   {
      <>;
   }
   exit;
}

if( $filesized < $filesizeb )
{
   unlink $compressed_fileb;
   unlink $orignal_file;
   rename( $compressed_filed, $orignal_file );
   print "$compressed_filed appears to be smaller\n";
   print "Renaming smallest file.\n";
   if( $bWaitForKeyPressBeforeExit == 1 )
   {
      <>;}

   exit;
}


