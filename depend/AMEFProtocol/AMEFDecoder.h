/*
	Copyright 2010, Sumeet Chhetri

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
/*
 * AMEFDecoder.h
 *
 *  Created on: Mar 7, 2011
 *      Author: chhetri.sumeet
 */

#ifndef AMEFDECODER_H_
#define AMEFDECODER_H_

#include "AMEFObject.h"
#include "iostream"
class AMEFDecoder
{


public:
	~AMEFDecoder();
	AMEFDecoder();
	static char* longTocharArray(long l,int ind)
		{
			char* result = new char[ind];
			for (int i = 0; i<ind; i++)
			{
				int offset = (ind - 1 - i) * 8;
	            result[i] = (char) ((l >> offset) & 0xFF);
			}
	        return result;
	    }

		static string longTocharArrayS(long l,int ind)
		{
			char* result = new char[ind];
			for (int i = 0; i<ind; i++)
			{
				int offset = (ind - 1 - i) * 8;
	            result[i] = (char) ((l >> offset) & 0xFF);
			}
			string tem(result);
			return tem;
	    }

		static char* intTocharArray(int l,int ind)
		{
			char* result = new char[ind];
			for (int i = 0; i<ind; i++)
			{
				int offset = (ind - 1 - i) * 8;
	            result[i] = (char) ((l >> offset) & 0xFF);
			}
	        return result;
	    }

		static char* intTocharArrayWI(int l)
		{
			int ind = 1;
			if(l<256)
				ind =1;
			else if(l<65536)
				ind = 2;
			else if(l<16777216)
				ind =3;
			else
				ind =4;
			char* result = new char[ind];
			for (int i = 0; i<ind; i++)
			{
				int offset = (ind - 1 - i) * 8;
	            result[i] = (char) ((l >> offset) & 0xFF);
			}
	        return result;
	    }

		static int charArrayToInt(char l[])
		{
			int t = 0;
			int ind = sizeof l;
	        for (int i = 0; i < ind; i++)
			{
	        	int offset = (ind -1 - i) * 8;
	        	t += (l[i] & 0x000000FF) << offset;
			}
	        return t;
	    }

		static int charArrayToInt(string l,int off,int ind)
		{
			int t = 0;
			for (int i = 0; i < ind; i++)
			{
	        	int offset = (ind -1 - i) * 8;
	        	t += (l[off+i] & 0x000000FF) << offset;
			}
	        return t;
	    }

		static long charArrayToLong(char l[])
		{
			long t = 0;
			int ind = sizeof l;
	        for (int i = 0; i < ind; i++)
			{
	        	int offset = (ind -1 - i) * 8;
	        	t += (l[i] & 0x000000FF) << offset;
			}
	        return t;
	    }
		static long charArrayToLong(char* l,int off,int ind)
		{
			long t = 0;
			for (int i = 0; i < ind; i++)
			{
	        	int offset = (ind -1 - i) * 8;
	        	t += (l[off+i] & 0x000000FF) << offset;
			}
	        return t;
	    }
		static long charArrayToLong(char* l,int ind)
		{
			long t = 0;
			for (int i = 0; i < ind; i++)
			{
	        	int offset = (ind -1 - i) * 8;
	        	t += (l[i] & 0x000000FF) << offset;
			}
	        return t;
	    }

		static string intTocharArrayS(int l, int ind)
		{
			char* result = new char[ind];
			for (int i = 0; i<ind; i++)
			{
				int offset = (ind - 1 - i) * 8;
	            result[i] = (char) ((l >> offset) & 0xFF);
			}
			string tem(result);
	        return tem;
	    }

	AMEFObject* decodeB(string buffer,bool considerLength,bool ignoreName)
	{
		position = 0;
		string strdata;
		if(considerLength)
		{
			strdata = buffer.substr(4);
		}
		else
		{
			strdata = buffer;
		}
		AMEFObject* AMEFObject = decodeSinglePacketBINNew(strdata,ignoreName);
		return AMEFObject;
	}

	AMEFObject* decodeBWL(string buffer,bool ignoreName)
	{
		position = 0;
		AMEFObject* AMEFObject = decodeSinglePacketBINNew(buffer,ignoreName);
		return AMEFObject;
	}



	/*AMEFObject decodeAMEFString(char* data,bool considerLength)
	{
		int startWith = 0;
		if(considerLength)
			startWith = 4;
		String strdata = new String(data);
		strdata = strdata.substring(startWith);
		AMEFObject AMEFObject = decodeSingleAMEFStringPacket(strdata);
		return AMEFObject;
	}

	AMEFObject decodeSingleAMEFStringPacket(String strdata)
	{
		AMEFObject aMEFObject = null;
		char type = strdata.charAt(0);
		if(type==AMEFObject::STRING_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 24) | ((strdata.charAt(2) & 0xff) << 16)
							| ((strdata.charAt(3) & 0xff) << 8) | ((strdata.charAt(4) & 0xff));
			aMEFObject->setLength(lengthm);
			String value = strdata.substring(5,5+lengthm);
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(5+lengthm);
		}
		else if(type==AMEFObject::STRING_65536_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 8) | (strdata.charAt(2) & 0xff);
			aMEFObject->setLength(lengthm);
			String value = strdata.substring(3,3+lengthm);
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(3+lengthm);
		}
		else if(type==AMEFObject::STRING_16777216_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 16)
							| ((strdata.charAt(2) & 0xff) << 8) | (strdata.charAt(3) & 0xff);
			aMEFObject->setLength(lengthm);
			String value = strdata.substring(3,3+lengthm);
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(3+lengthm);
		}
		else if(type==AMEFObject::DATE_TYPE || type==AMEFObject::STRING_256_TYPE || type==AMEFObject::DOUBLE_FLOAT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = strdata.charAt(1) & 0xff;
			aMEFObject->setLength(lengthm);
			String value = strdata.substring(2,2+lengthm);
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(2+lengthm);
		}
		else if(type==AMEFObject::VERY_SMALL_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			String value = (strdata.charAt(1) & 0xff) + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(2);
		}
		else if(type==AMEFObject::SMALL_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 8) | ((strdata.charAt(2) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(3);
		}
		else if(type==AMEFObject::BIG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 16) | ((strdata.charAt(2) & 0xff) << 8)
							| ((strdata.charAt(3) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(4);
		}
		else if(type==AMEFObject::INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 24) | ((strdata.charAt(2) & 0xff) << 16)
							| ((strdata.charAt(3) & 0xff) << 8) | ((strdata.charAt(4) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(5);
		}
		else if(type==AMEFObject::VS_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			long lengthm = ((strdata.charAt(1) & 0xff) << 32) | ((strdata.charAt(2) & 0xff) << 24)
							| ((strdata.charAt(3) & 0xff) << 16) | ((strdata.charAt(4) & 0xff) << 8)
							| ((strdata.charAt(5) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(6);
		}
		else if(type==AMEFObject::S_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			long lengthm = ((strdata.charAt(1) & 0xff) << 40) | ((strdata.charAt(2) & 0xff) << 32)
							| ((strdata.charAt(3) & 0xff) << 24)
							| ((strdata.charAt(4) & 0xff) << 16) | ((strdata.charAt(5) & 0xff) << 8)
							| ((strdata.charAt(6) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(7);
		}
		else if(type==AMEFObject::B_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			long lengthm = ((strdata.charAt(1) & 0xff) << 48) | ((strdata.charAt(2) & 0xff) << 40)
							| ((strdata.charAt(3) & 0xff) << 32)
							| ((strdata.charAt(4) & 0xff) << 24)
							| ((strdata.charAt(5) & 0xff) << 16) | ((strdata.charAt(6) & 0xff) << 8)
							| ((strdata.charAt(7) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(8);
		}
		else if(type==AMEFObject::LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			long lengthm = ((strdata.charAt(1) & 0xff) << 56) | ((strdata.charAt(2) & 0xff) << 48)
							| ((strdata.charAt(3) & 0xff) << 40)
							| ((strdata.charAt(4) & 0xff) << 32)
							| ((strdata.charAt(5) & 0xff) << 24)
							| ((strdata.charAt(6) & 0xff) << 16) | ((strdata.charAt(7) & 0xff) << 8)
							| ((strdata.charAt(8) & 0xff));
			String value = lengthm + "";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(9);
		}
		else if(type==AMEFObject::BOOLEAN_TYPE || type==AMEFObject::CHAR_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			aMEFObject->setLength(1);
			String value = strdata.charAt(1)+"";
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(2);
		}
		else if(type==AMEFObject::VS_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = charArrayToInt(strdata.substring(1,2).getchars());
			aMEFObject->setLength(lengthm);
			tempVal = strdata.substring(2,2+lengthm);
			while(!tempVal.equals(""))
			{
				AMEFObject obj = decodeSingleAMEFStringPacket(tempVal);
				aMEFObject->addPacket(obj);
			}
			tempVal = strdata.substring(2+lengthm);
		}
		else if(type==AMEFObject::S_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 8) | ((strdata.charAt(2) & 0xff));
			aMEFObject->setLength(lengthm);
			tempVal = strdata.substring(3,3+lengthm);
			while(!tempVal.equals(""))
			{
				AMEFObject obj = decodeSingleAMEFStringPacket(tempVal);
				aMEFObject->addPacket(obj);
			}
			tempVal = strdata.substring(3+lengthm);
		}
		else if(type==AMEFObject::B_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 16)
							| ((strdata.charAt(2) & 0xff) << 8) | ((strdata.charAt(3) & 0xff));
			aMEFObject->setLength(lengthm);
			tempVal = strdata.substring(4,4+lengthm);
			while(!tempVal.equals(""))
			{
				AMEFObject obj = decodeSingleAMEFStringPacket(tempVal);
				aMEFObject->addPacket(obj);
			}
			tempVal = strdata.substring(4+lengthm);
		}
		else if(type==AMEFObject::OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int lengthm = ((strdata.charAt(1) & 0xff) << 24) | ((strdata.charAt(2) & 0xff) << 16)
							| ((strdata.charAt(3) & 0xff) << 8) | ((strdata.charAt(4) & 0xff));
			aMEFObject->setLength(lengthm);
			tempVal = strdata.substring(5,5+lengthm);
			while(!tempVal.equals(""))
			{
				AMEFObject obj = decodeSingleAMEFStringPacket(tempVal);
				aMEFObject->addPacket(obj);
			}
			tempVal = strdata.substring(5+lengthm);
		}
		return aMEFObject;
	}

	*//**
	 * @param strdata
	 * @return AMEFObject
	 * @throws AMEFDecodeException
	 * decode the string to give the equivalent AMEFObject
	 * @throws UnsupportedEncodingException
	 *//*
	AMEFObject decodeSinglePacket(String strdata,bool ignoreName), UnsupportedEncodingException
	{
		AMEFObject aMEFObject = null;
		char type = strdata.charAt(0);
		if(type==AMEFObject::STRING_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<4)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 24) | ((length.getchars()[1] & 0xff) << 16)
								| ((length.getchars()[2] & 0xff) << 8) | ((length.getchars()[3] & 0xff));
				aMEFObject->setLength(lengthm);
				String value = strdata.substring(pos+1,pos+lengthm+1);
				aMEFObject->setValue(value,lengthm);
				tempVal = strdata.substring(pos+lengthm+1);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::STRING_65536_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<2)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 8) | ((length.getchars()[1] & 0xff));
				aMEFObject->setLength(lengthm);
				String value = strdata.substring(pos,pos+lengthm);
				aMEFObject->setValue(value,lengthm);
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::STRING_16777216_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<3)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 16) | ((length.getchars()[1] & 0xff) << 8)
								| ((length.getchars()[2] & 0xff));
				aMEFObject->setLength(lengthm);
				String value = strdata.substring(pos,pos+lengthm);
				aMEFObject->setValue(value,lengthm);
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::BOOLEAN_TYPE || type==AMEFObject::CHAR_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setLength(1);
				String value = "";
				if(!ignoreName)
				{
					value = strdata.substring(pos+1,pos+2);
					tempVal = strdata.substring(pos+2);
				}
				else
				{
					value = strdata.substring(pos,pos+1);
					tempVal = strdata.substring(pos+1);
				}
				aMEFObject->setValue(value,lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::STRING_256_TYPE || type==AMEFObject::DATE_TYPE || type==AMEFObject::DOUBLE_FLOAT_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<1)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = length.charAt(0) & 0xff;
				aMEFObject->setLength(lengthm);
				String value = strdata.substring(pos,pos+lengthm);
				aMEFObject->setValue(value,lengthm);
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::VERY_SMALL_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 1);
		}
		else if(type==AMEFObject::SMALL_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 2);
		}
		else if(type==AMEFObject::BIG_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 3);
		}
		else if(type==AMEFObject::INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 4);
		}
		else if(type==AMEFObject::VS_LONG_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 5);
		}
		else if(type==AMEFObject::S_LONG_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 6);
		}
		else if(type==AMEFObject::B_LONG_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 7);
		}
		else if(type==AMEFObject::LONG_INT_TYPE)
		{
			aMEFObject = getObject(strdata, type, ignoreName, 8);
		}
		else if(type==AMEFObject::VS_OBJECT_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<1)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0]) & 0xff);
				aMEFObject->setLength(lengthm);
				//
				//String value = strdata.substring(pos+1,pos+lengthm+1);
				tempVal  = strdata.substring(pos,pos+lengthm);
				while(!tempVal.equals(""))
				{
					AMEFObject obj = decodeSinglePacket(tempVal,ignoreName);
					aMEFObject->addPacket(obj);
				}
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::S_OBJECT_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<2)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 8) | ((length.getchars()[1]) & 0xff);
				aMEFObject->setLength(lengthm);
				//String value = strdata.substring(pos+1,pos+lengthm+1);
				//tempVal = value;
				tempVal  = strdata.substring(pos,pos+lengthm);
				while(!tempVal.equals(""))
				{
					AMEFObject obj = decodeSinglePacket(tempVal,ignoreName);
					aMEFObject->addPacket(obj);
				}
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::B_OBJECT_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<3)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 16)
								| ((length.getchars()[1] & 0xff) << 8) | ((length.getchars()[2]) & 0xff);
				aMEFObject->setLength(lengthm);
				//String value = strdata.substring(pos+1,pos+lengthm+1);
				//tempVal = value;
				tempVal  = strdata.substring(pos,pos+lengthm);
				while(!tempVal.equals(""))
				{
					AMEFObject obj = decodeSinglePacket(tempVal,ignoreName);
					aMEFObject->addPacket(obj);
				}
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		else if(type==AMEFObject::OBJECT_TYPE)
		{
			if(strdata.charAt(1)==',')
			{
				aMEFObject = new AMEFObject();
				aMEFObject->setType(type);
				int pos = 2;
				String name = "";
				if(!ignoreName)
				{
					while(strdata.charAt(pos)!=',')
					{
						name += strdata.charAt(pos++);
					}
					if(name.equals("") && strdata.charAt(2)!=',')
					{
						throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
					}
				}
				if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				aMEFObject->setName(buffer.substr(st,en-st));}
				String length = "";
				if(!ignoreName)pos++;
				while(length.length()<4)
				{
					length += strdata.charAt(pos++);
				}
				if(length.equals("") && strdata.charAt(3)!=',')
				{
					throw new AMEFDecodeException("Invalid character after length specifier, expected ,");
				}
				else if(pos>=strdata.length())
				{
					throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
				}
				int lengthm = ((length.getchars()[0] & 0xff) << 24) | ((length.getchars()[1] & 0xff) << 16)
								| ((length.getchars()[2] & 0xff) << 8) | ((length.getchars()[3]) & 0xff);
				aMEFObject->setLength(lengthm);
				//String value = strdata.substring(pos+1,pos+lengthm+1);
				//tempVal = value;
				tempVal  = strdata.substring(pos,pos+lengthm);
				while(!tempVal.equals(""))
				{
					AMEFObject obj = decodeSinglePacket(tempVal,ignoreName);
					aMEFObject->addPacket(obj);
				}
				tempVal = strdata.substring(pos+lengthm);
			}
			else
			{
				throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
			}
		}
		return aMEFObject;
	}

	AMEFObject getObject(String strdata,char type,bool ignoreName,int typ)
	{
		AMEFObject aMEFObject = null;
		if(strdata.charAt(1)==',')
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			int pos = 2;
			String name = "";
			if(!ignoreName)
			{
				while(strdata.charAt(pos)!=',')
				{
					name += strdata.charAt(pos++);
				}
				if(name.equals("") && strdata.charAt(2)!=',')
				{
					throw new AMEFDecodeException("Invalid character after name specifier, expected ,");
				}
			}
			if(pos>=strdata.length())
			{
				throw new AMEFDecodeException("Reached end of AMEF string, not found ,");
			}
			aMEFObject->setName(buffer.substr(st,en-st));}
			if(!ignoreName)pos++;
			String value = strdata.substring(pos,pos+typ);
			aMEFObject->setValue(value,lengthm);
			tempVal = strdata.substring(pos+typ);
		}
		else
		{
			throw new AMEFDecodeException("Invalid character after type specifier, expected ,");
		}
		return aMEFObject;
	}*/

	int position;
	AMEFObject* decodeSinglePacketB(string buffer,bool ignoreName)
	{
		char type = (char)buffer[position];
		AMEFObject *aMEFObject = NULL;
		int st, en;
		if(type==AMEFObject::NULL_STRING || type==AMEFObject::NULL_DATE || type==AMEFObject::NULL_NUMBER
				|| type==AMEFObject::NULL_BOOL || type==AMEFObject::NULL_CHAR)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
		}
		else if(type==AMEFObject::STRING_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,4);
			aMEFObject->setLength(lengthm);
			position += 4;
			string value = buffer.substr(position,lengthm);
			aMEFObject->setValue(value);
			position += 5+lengthm;
		}
		else if(type==AMEFObject::STRING_65536_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,2);
			aMEFObject->setLength(lengthm);
			position += 2;
			string value = buffer.substr(position,lengthm);
			aMEFObject->setValue(value);
			position += 3+lengthm;
		}
		else if(type==AMEFObject::STRING_16777216_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,3);
			aMEFObject->setLength(lengthm);
			position += 3;
			string value = buffer.substr(position,lengthm);
			aMEFObject->setValue(value);
			position += 4+lengthm;
		}
		else if(type==AMEFObject::DATE_TYPE || type==AMEFObject::STRING_256_TYPE || type==AMEFObject::DOUBLE_FLOAT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,1);
			aMEFObject->setLength(lengthm);
			position++;
			string value = buffer.substr(position,lengthm);
			aMEFObject->setValue(value);
			position += lengthm;
		}
		else if(type==AMEFObject::VERY_SMALL_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(1);
			aMEFObject->pushChar(buffer[position]);
			position += 1;
		}
		else if(type==AMEFObject::SMALL_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(2);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			position += 2;
		}
		else if(type==AMEFObject::BIG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(3);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			position += 3;
		}
		else if(type==AMEFObject::INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(4);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			aMEFObject->pushChar(buffer[position+3]);
			position += 4;
		}
		else if(type==AMEFObject::VS_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(5);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			aMEFObject->pushChar(buffer[position+3]);
			aMEFObject->pushChar(buffer[position+4]);
			position += 5;
		}
		else if(type==AMEFObject::S_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(6);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			aMEFObject->pushChar(buffer[position+3]);
			aMEFObject->pushChar(buffer[position+4]);
			aMEFObject->pushChar(buffer[position+5]);
			position += 6;
		}
		else if(type==AMEFObject::B_LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(7);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			aMEFObject->pushChar(buffer[position+3]);
			aMEFObject->pushChar(buffer[position+4]);
			aMEFObject->pushChar(buffer[position+5]);
			aMEFObject->pushChar(buffer[position+6]);
			position += 7;
		}
		else if(type==AMEFObject::LONG_INT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(8);
			aMEFObject->pushChar(buffer[position]);
			aMEFObject->pushChar(buffer[position+1]);
			aMEFObject->pushChar(buffer[position+2]);
			aMEFObject->pushChar(buffer[position+3]);
			aMEFObject->pushChar(buffer[position+4]);
			aMEFObject->pushChar(buffer[position+5]);
			aMEFObject->pushChar(buffer[position+6]);
			aMEFObject->pushChar(buffer[position+7]);
			position += 8;
		}
		else if(type==AMEFObject::BOOLEAN_TYPE || type==AMEFObject::CHAR_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			aMEFObject->setLength(1);
			aMEFObject->pushChar(buffer[position]);
			position += 1;
		}
		else if(type==AMEFObject::VS_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,1);
			aMEFObject->setLength(lengthm);
			position++;
			while(position<buffer.length())
			{
				AMEFObject *obj = decodeSinglePacketB(buffer,ignoreName);
				aMEFObject->addPacket(obj);
			}
		}
		else if(type==AMEFObject::S_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,2);
			aMEFObject->setLength(lengthm);
			//char* value = new char[lengthm];
			//System.arraycopy(buffer, 3, value, 0, lengthm);
			position += 2;
			while(position<buffer.length())
			{
				AMEFObject* obj = decodeSinglePacketB(buffer,ignoreName);
				aMEFObject->addPacket(obj);
			}
		}
		else if(type==AMEFObject::B_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,3);
			aMEFObject->setLength(lengthm);
			position += 3;
			while(position<buffer.length())
			{
				AMEFObject* obj = decodeSinglePacketB(buffer,ignoreName);
				aMEFObject->addPacket(obj);
			}
		}
		else if(type==AMEFObject::OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			aMEFObject->setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				st = position;
				while(buffer[position++]!=44){}
				en = position - 1;
				if(en>st){

				aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,4);
			aMEFObject->setLength(lengthm);
			position += 4;
			while(position<buffer.length())
			{
				AMEFObject* obj = decodeSinglePacketB(buffer,ignoreName);
				aMEFObject->addPacket(obj);
			}
		}
		return aMEFObject;
	}

	void addPrimitive(AMEFObject* aMEFObject,string buffer,bool ignoreName)
	{
		char type = (char)buffer[position];
		if(type==AMEFObject::NULL_STRING || type==AMEFObject::NULL_DATE || type==AMEFObject::NULL_NUMBER
				|| type==AMEFObject::NULL_BOOL || type==AMEFObject::NULL_CHAR)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			if(name=="")aMEFObject->addNullPacket(type);else aMEFObject->addNullPacket(type,name);
		}
		else if(type==AMEFObject::STRING_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,4);
			position += 4;
			string value = buffer.substr(position,lengthm);
			if(name=="")aMEFObject->addPacket(value);else aMEFObject->addPacket(value,name);
			position += 5+lengthm;
		}
		else if(type==AMEFObject::STRING_65536_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,2);
			position += 2;
			string value = buffer.substr(position,lengthm);
			if(name=="")aMEFObject->addPacket(value);else aMEFObject->addPacket(value,name);
			position += 3+lengthm;
		}
		else if(type==AMEFObject::STRING_16777216_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,3);
			position += 3;
			string value = buffer.substr(position,lengthm);
			if(name=="")aMEFObject->addPacket(value);else aMEFObject->addPacket(value,name);
			position += 4+lengthm;
		}
		else if(type==AMEFObject::DATE_TYPE || type==AMEFObject::STRING_256_TYPE || type==AMEFObject::DOUBLE_FLOAT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			int lengthm = charArrayToInt(buffer,position,1);
			//aMEFObject.setLength(lengthm);
			position++;
			string value = buffer.substr(position,lengthm);
			AMEFObject* temp = NULL;
			if(name=="")
			{
				temp = aMEFObject->addPacket(value);
				temp->setType(type);
			}
			else
			{
				aMEFObject->addPacket(value,name);
				temp = aMEFObject->getPackets().at(aMEFObject->getPackets().size()-1);
				temp->setType(type);
			}
			position += lengthm;
		}
		else if(type==AMEFObject::VERY_SMALL_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[1];
			a[0] = buffer[position];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,1));else aMEFObject->addPacket(charArrayToLong(a,1),name);
			position += 1;
		}
		else if(type==AMEFObject::SMALL_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[2];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,2));else aMEFObject->addPacket(charArrayToLong(a,2),name);
			position += 2;
		}
		else if(type==AMEFObject::BIG_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[3];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,3));else aMEFObject->addPacket(charArrayToLong(a,3),name);
			position += 3;
		}
		else if(type==AMEFObject::INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[4];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			a[3] = buffer[position+3];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,4));else aMEFObject->addPacket(charArrayToLong(a,4),name);
			position += 4;
		}
		else if(type==AMEFObject::VS_LONG_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[5];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			a[3] = buffer[position+3];
			a[4] = buffer[position+4];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,5));else aMEFObject->addPacket(charArrayToLong(a,5),name);
			position += 5;
		}
		else if(type==AMEFObject::S_LONG_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[6];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			a[3] = buffer[position+3];
			a[4] = buffer[position+4];
			a[5] = buffer[position+5];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,5));else aMEFObject->addPacket(charArrayToLong(a,6),name);
			position += 6;
		}
		else if(type==AMEFObject::B_LONG_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[7];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			a[3] = buffer[position+3];
			a[4] = buffer[position+4];
			a[5] = buffer[position+5];
			a[6] = buffer[position+6];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,7));else aMEFObject->addPacket(charArrayToLong(a,7),name);
			position += 7;
		}
		else if(type==AMEFObject::LONG_INT_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			char a[8];
			a[0] = buffer[position];
			a[1] = buffer[position+1];
			a[2] = buffer[position+2];
			a[3] = buffer[position+3];
			a[4] = buffer[position+4];
			a[5] = buffer[position+5];
			a[6] = buffer[position+6];
			a[7] = buffer[position+7];
			if(name=="")aMEFObject->addPacket(charArrayToLong(a,8));else aMEFObject->addPacket(charArrayToLong(a,8),name);
			position += 8;
		}
		else if(type==AMEFObject::BOOLEAN_TYPE || type==AMEFObject::CHAR_TYPE)
		{
			string name;
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){name = (buffer.substr(st,en-st));}
			}
			else
				position++;
			if(type==AMEFObject::BOOLEAN_TYPE)
				if(name=="")aMEFObject->addPacket((buffer[position]=='1'?true:false));else aMEFObject->addPacket((buffer[position]=='1'?true:false));
			else
				if(name=="")aMEFObject->addPacket(buffer[position]);else aMEFObject->addPacket(buffer[position]);
			position += 1;
		}
		//aMEFObject.position = position;
	}

	AMEFObject* decodeSinglePacketBINNew(string buffer,bool ignoreName)
	{
		char type = (char)buffer[position];
		AMEFObject* aMEFObject = NULL;
		if(type==AMEFObject::VS_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			//aMEFObject.setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			//int lengthm = charArrayToInt(buffer,position,1);
			//aMEFObject.setLength(lengthm);
			position++;
			while(position<buffer.length())
			{
				addPrimitive(aMEFObject, buffer,ignoreName);
				//position = aMEFObject.position;
			}
		}
		else if(type==AMEFObject::S_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			//aMEFObject.setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			//int lengthm = charArrayToInt(buffer,position,2);
			//aMEFObject.setLength(lengthm);
			//byte[] value = new byte[lengthm];
			//System.arraycopy(buffer, 3, value, 0, lengthm);
			position += 2;
			while(position<buffer.length())
			{
				addPrimitive(aMEFObject, buffer,ignoreName);
				//position = aMEFObject.position;
			}
		}
		else if(type==AMEFObject::B_OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			//aMEFObject.setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			//int lengthm = charArrayToInt(buffer,position,3);
			//aMEFObject.setLength(lengthm);
			//byte[] value = new byte[lengthm];
			//System.arraycopy(buffer, 4, value, 0, lengthm);
			position += 3;
			while(position<buffer.length())
			{
				addPrimitive(aMEFObject, buffer,ignoreName);
				//position = aMEFObject.position;
			}
		}
		else if(type==AMEFObject::OBJECT_TYPE)
		{
			aMEFObject = new AMEFObject();
			//aMEFObject.setType(type);
			if(!ignoreName)
			{
				while(buffer[position++]!=44){}
				int st = position;
				while(buffer[position++]!=44){}
				int en = position - 1;
				if(en>st){aMEFObject->setName(buffer.substr(st,en-st));}
			}
			else
				position++;
			//int lengthm = charArrayToInt(buffer,position,4);
			//aMEFObject.setLength(lengthm);
			//byte[] value = new byte[lengthm];
			//System.arraycopy(buffer, 5, value, 0, lengthm);
			position += 4;
			while(position<buffer.length())
			{
				addPrimitive(aMEFObject, buffer,ignoreName);
				//position = aMEFObject.position;
			}
		}
		return aMEFObject;
	}

};

#endif /* AMEFDECODER_H_ */
