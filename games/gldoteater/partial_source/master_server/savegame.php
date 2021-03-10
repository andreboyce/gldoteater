<?php
   //echo $_SERVER['REQUEST_URI'];
   //"game=doteater+version=0.4.5.3+nick=andreboyce+IP=127.0.0.1+port=7654"
   //"game=doteater&version=0.4.5.3&nick=andreboyce&IP=127.0.0.1&port=7654"

   function saveGame( $filename = "games.txt" )
   {
       $game        = $_REQUEST['game'];
       $port        = $_REQUEST['port'];
       $nick        = $_REQUEST['nick'];
       $version     = $_REQUEST['version'];       
       $IP          = $_SERVER['REMOTE_ADDR'];
       
       $game_string = "game=$game+version=$version+nick=$nick+IP=$IP+port=$port";
       echo $game_string;
       file_put_contents( $filename, $game_string );
       return $game_string;
   }

   saveGame();   
?>
