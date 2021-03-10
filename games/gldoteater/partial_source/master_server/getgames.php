<?php
    //$games = "game=doteater+version=0.4.5.3+nick=andreboyce+IP=127.0.0.1+port=7654";

   function getGames( $filename = "games.txt" )
   {
       $games = explode("\n", file_get_contents($filename));
       return $games;
   }

   $games = getGames();
   if( count($games) > 0 )
   {
       echo "{$games[0]}";
   }
   else
   {
       //var_dump( $games );
   }
   
?>
