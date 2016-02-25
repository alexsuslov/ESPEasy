// Profiling
// @auth Alex Suslov <suslov@me.com>
// @auth Victor Brutskiy <4refr0nt@gmail.com>

#ifdefine MEMORY_PROFILING
// Log free memory for procedure
// uptime : Free Mem : procedure : vars
void Serial_memory_log( String name, String vars){

  Serial.print( wdcounter / 2);
  Serial.print( ":" );
  Serial.print( ESP.getFreeHeap());
  Serial.print( ":" );
  Serial.print(name + ":");
  Serial.println(var);
}

#endif //MEMORY_PROFILING

