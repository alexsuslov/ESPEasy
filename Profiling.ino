// Profiling
// @auth Alex Suslov <suslov@me.com>
// @auth Victor Brutskiy <4refr0nt@gmail.com>

#if MEMORY_PROFILING
// Log free memory for procedure
// uptime : Free Mem : procedure : vars
void Serial_memory_log( String name, String vars){

  char str[60];
  str[0] = 0;
  unsigned long mem = FreeMem();
  sprintf_P(str, PSTR("MemP : Uptime %u FreeMem %u MemLeak %d "), wdcounter / 2, mem, lastMem - mem );
  Serial.print( String(str) );
  Serial.print(" : " + name + " : ");
  Serial.println(vars);
  lastMem = mem;
}

#endif //MEMORY_PROFILING

