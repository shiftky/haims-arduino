//===========================//
//                           //
//   HAIMS - IR.ino          //
//                           //
//===========================//

void IRDump(decode_results *results)
{
  int count = results->rawlen;
  Serial.print("Raw(");
  Serial.print(count, DEC);
  Serial.print("):");

  for (int i = 1; i < count; i++) {
    Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    if (i != count-1 ){
      Serial.print(",");
    }
  }
  Serial.println("");
}
