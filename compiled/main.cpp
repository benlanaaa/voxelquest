
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif



#include <SDKDDKVer.h>


#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <list>
#include <cstddef>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <ios>
#include <cstdlib>

#include <iomanip>
#include <map>
#include <ctime>


#include <stdlib.h>
#include <math.h>
#include <tchar.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <conio.h>



#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/freeglut.h>

#pragma comment(lib, "glew32.lib")



#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadPool.h"
#include "Poco/Base64Decoder.h"


using Poco::Net::ServerSocket;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::SystemException;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::Runnable;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

using Poco::Base64Decoder;


/*
#pragma comment (lib, "olepro32.lib")   // for IPicture COM interface support
#include <windows.h>
#include <olectl.h.>    // for OleLoadPicture() and IPicture COM interface
*/

//#define DEBUG_MODE 1

float MAX_GPU_MEM = 1536.0f;
float TOT_GPU_MEM_USAGE = 0.0f;

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


#if defined(_DEBUG)
#include <crtdbg.h>
#endif

using namespace std;


#define _USE_MATH_DEFINES

typedef unsigned int uint;


// Win32 incompatibilities
#if defined(WIN32) && !defined(__GNUC__)
	#define wcsncasecmp _wcsnicmp
	static inline bool isnan(double x) { return x != x; }
	static inline bool isinf(double x) { return !isnan(x) && isnan(x - x); }
#endif

// Linux compile fix - from quaker66
#ifdef __GNUC__
	// #include <cstring>
	// #include <cstdlib>
#endif

// Mac compile fixes - from quaker66, Lion fix by dabrahams
#if defined(__APPLE__) && __DARWIN_C_LEVEL < 200809L || (defined(WIN32) && defined(__GNUC__))
	#include <wctype.h>
	#include <wchar.h>
	
	static inline int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n)
	{
		int lc1  = 0;
		int lc2  = 0;

		while (n--)
		{
			lc1 = towlower (*s1);
			lc2 = towlower (*s2);

			if (lc1 != lc2)
				return (lc1 - lc2);

			if (!lc1)
				return 0;

			++s1;
			++s2;
		}

		return 0;
	}
#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool simplejson_wcsnlen(const wchar_t *s, size_t n) {
	if (s == 0)
		return false;

	const wchar_t *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) return false;
	}

	return true;
}

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};



bool PROG_ACTIVE = true;



 

struct iVector4 {
	int x;
	int y;
	int z;
	int w;
};
struct fVector4 {
	float x;
	float y;
	float z;
	float w;
};

struct iVector3 {
	int x;
	int y;
	int z;
};
struct fVector3 {
	float x;
	float y;
	float z;
};


struct iVector2 {
	int x;
	int y;
};
struct fVector2 {
	float x;
	float y;
};

struct iBoundingBox {
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct fBoundingBox {
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};



class FIVector4 {
private:
    iVector4 iv4;
    fVector4 fv4;

    iVector3 iv3;
    fVector3 fv3;
    
    iVector2 iv2;
    fVector2 fv2;

public:
    
    FIVector4() {
        iv4.x = 0;
        iv4.y = 0;
        iv4.z = 0;
        iv4.w = 0;
        fv4.x = 0.0;
        fv4.y = 0.0;
        fv4.z = 0.0;
        fv4.w = 0.0;
    }

    void setIXYZW(int x, int y, int z, int w) {
        iv4.x = x;
        iv4.y = y;
        iv4.z = z;
        iv4.w = w;
        fv4.x = (float)x;
        fv4.y = (float)y;
        fv4.z = (float)z;
        fv4.w = (float)w;
    }
    void setIXYZ(int x, int y, int z) {
        iv4.x = x;
        iv4.y = y;
        iv4.z = z;
        fv4.x = (float)x;
        fv4.y = (float)y;
        fv4.z = (float)z;
    }
    void setIXY(int x, int y) {
        iv4.x = x;
        iv4.y = y;
        fv4.x = (float)x;
        fv4.y = (float)y;
    }
    void setIX(int x) {
        iv4.x = x;
        fv4.x = (float)x;
    }
    void setIY(int y) {
        iv4.y = y;
        fv4.y = (float)y;
    }
    void setIZ(int z) {
        iv4.z = z;
        fv4.z = (float)z;
    }
    void setIW(int w) {
        iv4.w = w;
        fv4.w = (float)w;
    }

    void copyFrom(FIVector4* cf) {
        iv4.x = cf->getIX();
        iv4.y = cf->getIY();
        iv4.z = cf->getIZ();
        iv4.w = cf->getIW();

        fv4.x = cf->getFX();
        fv4.y = cf->getFY();
        fv4.z = cf->getFZ();
        fv4.w = cf->getFW();
    }



    void setFXYZW(float x, float y, float z, float w) {
        fv4.x = x;
        fv4.y = y;
        fv4.z = z;
        fv4.w = w;
        iv4.x = (int)x;
        iv4.y = (int)y;
        iv4.z = (int)z;
        iv4.w = (int)w;
    }
    void setFXYZ(float x, float y, float z) {
        fv4.x = x;
        fv4.y = y;
        fv4.z = z;
        iv4.x = (int)x;
        iv4.y = (int)y;
        iv4.z = (int)z;
    }

    void setFXYZRef(FIVector4 * v1) {
        fv4.x = v1->getFX();
        fv4.y = v1->getFY();
        fv4.z = v1->getFZ();
        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }

    void setFXY(float x, float y) {
        fv4.x = x;
        fv4.y = y;
        iv4.x = (int)x;
        iv4.y = (int)y;
    }
    void setFX(float x) {
        fv4.x = x;
        iv4.x = (int)x;
    }
    void setFY(float y) {
        fv4.y = y;
        iv4.y = (int)y;
    }
    void setFZ(float z) {
        fv4.z = z;
        iv4.z = (int)z;
    }
    void setFW(float w) {
        fv4.w = w;
        iv4.w = (int)w;
    }

    void addXYZ(float scalar, float multiplier=1.0f) {
        fv4.x += scalar*multiplier;
        fv4.y += scalar*multiplier;
        fv4.z += scalar*multiplier;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void addXYZ(float scalarX, float scalarY, float scalarZ, float multiplier=1.0f) {
        fv4.x += scalarX*multiplier;
        fv4.y += scalarY*multiplier;
        fv4.z += scalarZ*multiplier;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void addXYZRef(FIVector4 *scalar, float multiplier=1.0f) {
        fv4.x += scalar->getFX()*multiplier;
        fv4.y += scalar->getFY()*multiplier;
        fv4.z += scalar->getFZ()*multiplier;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }



    void multXYZ(float scalar) {
        fv4.x *= scalar;
        fv4.y *= scalar;
        fv4.z *= scalar;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void multXYZ(float scalarX, float scalarY, float scalarZ) {
        fv4.x *= scalarX;
        fv4.y *= scalarY;
        fv4.z *= scalarZ;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void multXYZ(FIVector4 *scalar) {
        fv4.x *= scalar->getFX();
        fv4.y *= scalar->getFY();
        fv4.z *= scalar->getFZ();

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }


    void intDivXYZ(int scalar) {
        iv4.x /= scalar;
        iv4.y /= scalar;
        iv4.z /= scalar;

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;
        fv4.z = (float)iv4.z;

    }


    void minXYZ(FIVector4 *v1, FIVector4 *v2) {
        fv4.x = std::min(v1->getFX(), v2->getFX());
        fv4.y = std::min(v1->getFY(), v2->getFY());
        fv4.z = std::min(v1->getFZ(), v2->getFZ());

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }

    void maxXYZ(FIVector4 *v1, FIVector4 *v2) {
        fv4.x = std::max(v1->getFX(), v2->getFX());
        fv4.y = std::max(v1->getFY(), v2->getFY());
        fv4.z = std::max(v1->getFZ(), v2->getFZ());

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }

    void clampXYZ(FIVector4 *minV, FIVector4 *maxV) {
        if (fv4.x < minV->getFX()) {
            fv4.x = minV->getFX();
        }
        if (fv4.y < minV->getFY()) {
            fv4.y = minV->getFY();
        }
        if (fv4.z < minV->getFZ()) {
            fv4.z = minV->getFZ();
        }
        if (fv4.x > maxV->getFX()) {
            fv4.x = maxV->getFX();
        }
        if (fv4.y > maxV->getFY()) {
            fv4.y = maxV->getFY();
        }
        if (fv4.z > maxV->getFZ()) {
            fv4.z = maxV->getFZ();
        }

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }

    bool inBoundsXYZ(FIVector4 *minV, FIVector4 *maxV) {
        if (fv4.x < minV->getFX()) {
            return false;
        }
        if (fv4.y < minV->getFY()) {
            return false;
        }
        if (fv4.z < minV->getFZ()) {
            return false;
        }
        if (fv4.x >= maxV->getFX()) {
            return false;
        }
        if (fv4.y >= maxV->getFY()) {
            return false;
        }
        if (fv4.z >= maxV->getFZ()) {
            return false;
        }

        return true;
    }

    float distance(FIVector4 *otherVec) {

        float dx = fv4.x - otherVec->getFX();
        float dy = fv4.y - otherVec->getFY();
        float dz = fv4.z - otherVec->getFZ();

        return sqrt(dx*dx + dy*dy + dz*dz);
    }

    void normalize() {
        float len = sqrt(fv4.x*fv4.x + fv4.y*fv4.y + fv4.z*fv4.z);

        fv4.x = fv4.x/len;
        fv4.y = fv4.y/len;
        fv4.z = fv4.z/len;


    }




    iVector4* getIXYZW() {
        return &iv4;
    }
    fVector4* getFXYZW() {
        return &fv4;
    }

    iVector3* getIXYZ() {
        iv3.x = iv4.x;
        iv3.y = iv4.y;
        iv3.z = iv4.z;
        return &iv3;
    }
    fVector3* getFXYZ() {
        fv3.x = fv4.x;
        fv3.y = fv4.y;
        fv3.z = fv4.z;
        return &fv3;
    }

    iVector2* getIXY() {
        iv2.x = iv4.x;
        iv2.y = iv4.y;
        return &iv2;
    }
    fVector2* getFXY() {
        fv2.x = fv4.x;
        fv2.y = fv4.y;
        return &fv2;
    }

    int getIX() {
        return iv4.x;
    }
    int getIY() {
        return iv4.y;
    }
    int getIZ() {
        return iv4.z;
    }
    int getIW() {
        return iv4.w;
    }

    float getFX() {
        return fv4.x;
    }
    float getFY() {
        return fv4.y;
    }
    float getFZ() {
        return fv4.z;
    }
    float getFW() {
        return fv4.w;
    }




};



class Vector3  {
public:
	
	float e[3];

    Vector3() { e[0] = 0; e[1] = 0; e[2] = 0;}
    Vector3(float e0, float e1, float e2) {e[0]=e0; e[1]=e1; e[2]=e2;}
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    void setX(float a) { e[0] = a; }
    void setY(float a) { e[1] = a; }
    void setZ(float a) { e[2] = a; }
	void setXYZ(float a, float b, float c) {
		e[0] = a;
		e[1] = b;
		e[2] = c;
	}

    inline Vector3(const Vector3 &v) {
	 e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];
    }

    const Vector3& operator+() const { return *this; }
    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float& operator[](int i) {  return e[i]; }
    float operator[](int i) const { return e[i];}

    Vector3& operator+=(const Vector3 &v2);
    Vector3& operator-=(const Vector3 &v2);
    Vector3& operator*=(const float t);
    Vector3& operator/=(const float t);



    float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    float squaredLength() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

    void makeUnitVector();




    float minComponent() const { return e[indexOfMinComponent()]; }
    float maxComponent() const { return e[indexOfMaxComponent()]; }
    float maxAbsComponent() const { return e[indexOfMaxAbsComponent()]; }
    float minAbsComponent() const { return e[indexOfMinAbsComponent()]; }
    int indexOfMinComponent() const {
	return (e[0]< e[1] && e[0]< e[2]) ? 0 : (e[1] < e[2] ? 1 : 2);
    }

    int indexOfMinAbsComponent() const {
	if (fabs(e[0]) < fabs(e[1]) && fabs(e[0]) < fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) < fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    int indexOfMaxComponent() const {
	return (e[0]> e[1] && e[0]> e[2]) ? 0 : (e[1] > e[2] ? 1 : 2);
    }

    int indexOfMaxAbsComponent() const {
	if (fabs(e[0]) > fabs(e[1]) && fabs(e[0]) > fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) > fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    
};



inline bool operator==(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]==t2[0])&&(t1[1]==t2[1])&&(t1[2]==t2[2]));
}

inline bool operator!=(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]!=t2[0])||(t1[1]!=t2[1])||(t1[2]!=t2[2]));
}

inline std::istream &operator>>(std::istream &is, Vector3 &t) {
   is >> t[0] >> t[1] >> t[2];
   return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3 &t) {
   os << t[0] << " " << t[1] << " " << t[2];
   return os;
}

inline Vector3 unitVector(const Vector3& v) {
    float k = 1.0f / sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
    return Vector3(v.e[0]*k, v.e[1]*k, v.e[2]*k);
}

inline void Vector3::makeUnitVector() {
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3 operator*(float t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, float t) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator/(const Vector3 &v, float t) {
    return Vector3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}


inline float dot(const Vector3 &v1, const Vector3 &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
		      (v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2]),
		      (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


inline Vector3& Vector3::operator+=(const Vector3 &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

inline Vector3& Vector3::operator/=(const float t) {
    e[0]  /= t;
    e[1]  /= t;
    e[2]  /= t;
    return *this;
}

inline Vector3 reflect(const Vector3& in, const Vector3& normal)
{
  // assumes unit length normal
  return in - normal * (2 * dot(in, normal));
}
 


enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object };

class JSONValue;

typedef std::vector<JSONValue*> JSONArray;
typedef std::map<std::wstring, JSONValue*> JSONObject;

class JSONValue
{
	
	public:
		JSONValue(/*NULL*/);
		JSONValue(const wchar_t *m_char_value);
		JSONValue(const std::wstring &m_string_value);
		JSONValue(bool m_bool_value);
		JSONValue(double m_number_value);
		JSONValue(const JSONArray &m_array_value);
		JSONValue(const JSONObject &m_object_value);
		~JSONValue();

		bool IsNull() const;
		bool IsString() const;
		bool IsBool() const;
		bool IsNumber() const;
		bool IsArray() const;
		bool IsObject() const;
		
		const std::wstring &AsString() const;
		bool AsBool() const;
		double AsNumber() const;
		const JSONArray &AsArray() const;
		const JSONObject &AsObject() const;

		std::size_t CountChildren() const;
		bool HasChild(std::size_t index) const;
		JSONValue *Child(std::size_t index);
		bool HasChild(const wchar_t* name) const;
		JSONValue *Child(const wchar_t* name);

		std::wstring Stringify() const;

		static JSONValue *Parse(const wchar_t **data);

		static std::wstring StringifyString(const std::wstring &str);
	
		JSONType type;
		std::wstring string_value;
		bool bool_value;
		double number_value;
		JSONArray array_value;
		JSONObject object_value;
};



class JSON
{
	
	public:
		static JSONValue* Parse(const char *data);
		static JSONValue* Parse(const wchar_t *data);
		static std::wstring Stringify(const JSONValue *value);

		static bool SkipWhitespace(const wchar_t **data);
		static bool ExtractString(const wchar_t **data, std::wstring &str);
		static double ParseInt(const wchar_t **data);
		static double ParseDecimal(const wchar_t **data);

		JSON();
};




// Macros to free an array/object
#define FREE_ARRAY(x) { JSONArray::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete *iter; } }
#define FREE_OBJECT(x) { JSONObject::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete (*iter).second; } }

/**
 * Parses a JSON encoded value to a JSONValue object
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the data
 *
 * @return JSONValue* Returns a pointer to a JSONValue object on success, NULL on error
 */
JSONValue *JSONValue::Parse(const wchar_t **data)
{
	// Is it a string?
	if (**data == '"')
	{
		std::wstring str;
		if (!JSON::ExtractString(&(++(*data)), str))
			return NULL;
		else
			return new JSONValue(str);
	}
	
	// Is it a boolean?
	else if ((simplejson_wcsnlen(*data, 4) && wcsncasecmp(*data, L"true", 4) == 0) || (simplejson_wcsnlen(*data, 5) && wcsncasecmp(*data, L"false", 5) == 0))
	{
		bool value = wcsncasecmp(*data, L"true", 4) == 0;
		(*data) += value ? 4 : 5;
		return new JSONValue(value);
	}
	
	// Is it a null?
	else if (simplejson_wcsnlen(*data, 4) && wcsncasecmp(*data, L"null", 4) == 0)
	{
		(*data) += 4;
		return new JSONValue();
	}
	
	// Is it a number?
	else if (**data == L'-' || (**data >= L'0' && **data <= L'9'))
	{
		// Negative?
		bool neg = **data == L'-';
		if (neg) (*data)++;

		double number = 0.0;

		// Parse the whole part of the number - only if it wasn't 0
		if (**data == L'0')
			(*data)++;
		else if (**data >= L'1' && **data <= L'9')
			number = JSON::ParseInt(data);
		else
			return NULL;
		
		// Could be a decimal now...
		if (**data == '.')
		{
			(*data)++;

			// Not get any digits?
			if (!(**data >= L'0' && **data <= L'9'))
				return NULL;
			
			// Find the decimal and sort the decimal place out
			// Use ParseDecimal as ParseInt won't work with decimals less than 0.1
			// thanks to Javier Abadia for the report & fix
			double decimal = JSON::ParseDecimal(data);
			
			// Save the number
			number += decimal;
		}

		// Could be an exponent now...
		if (**data == L'E' || **data == L'e')
		{
			(*data)++;

			// Check signage of expo
			bool neg_expo = false;
			if (**data == L'-' || **data == L'+')
			{
				neg_expo = **data == L'-';
				(*data)++;
			}
			
			// Not get any digits?
			if (!(**data >= L'0' && **data <= L'9'))
				return NULL;

			// Sort the expo out
			double expo = JSON::ParseInt(data);
			for (double i = 0.0; i < expo; i++)
				number = neg_expo ? (number / 10.0) : (number * 10.0);
		}

		// Was it neg?
		if (neg) number *= -1;

		return new JSONValue(number);
	}

	// An object?
	else if (**data == L'{')
	{
		JSONObject object;
		
		(*data)++;
	
		while (**data != 0)
		{
			// Whitespace at the start?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Special case - empty object
			if (object.size() == 0 && **data == L'}')
			{
				(*data)++;
				return new JSONValue(object);
			}
			
			// We want a string now...
			std::wstring name;
			if (!JSON::ExtractString(&(++(*data)), name))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Need a : now
			if (*((*data)++) != L':')
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// The value is here			
			JSONValue *value = Parse(data);
			if (value == NULL)
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Add the name:value
			if (object.find(name) != object.end())
				delete object[name];
			object[name] = value;
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// End of object?
			if (**data == L'}')
			{
				(*data)++;
				return new JSONValue(object);
			}
			
			// Want a , now
			if (**data != L',')
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			(*data)++;
		}
		
		// Only here if we ran out of data
		FREE_OBJECT(object);
		return NULL;
	}
	
	// An array?
	else if (**data == L'[')
	{
		JSONArray array;
		
		(*data)++;
		
		while (**data != 0)
		{
			// Whitespace at the start?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// Special case - empty array
			if (array.size() == 0 && **data == L']')
			{
				(*data)++;
				return new JSONValue(array);
			}
			
			// Get the value
			JSONValue *value = Parse(data);
			if (value == NULL)
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// Add the value
			array.push_back(value);
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// End of array?
			if (**data == L']')
			{
				(*data)++;
				return new JSONValue(array);
			}
			
			// Want a , now
			if (**data != L',')
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			(*data)++;
		}
		
		// Only here if we ran out of data
		FREE_ARRAY(array);
		return NULL;
	}
	
	// Ran out of possibilites, it's bad!
	else
	{
		return NULL;
	}
}

/**
 * Basic constructor for creating a JSON Value of type NULL
 *
 * @access public
 */
JSONValue::JSONValue(/*NULL*/)
{
	type = JSONType_Null;
}

/**
 * Basic constructor for creating a JSON Value of type String
 *
 * @access public
 *
 * @param wchar_t* m_char_value The string to use as the value
 */
JSONValue::JSONValue(const wchar_t *m_char_value)
{
	type = JSONType_String;
	string_value = std::wstring(m_char_value);
}

/**
 * Basic constructor for creating a JSON Value of type String
 *
 * @access public
 *
 * @param std::wstring m_string_value The string to use as the value
 */
JSONValue::JSONValue(const std::wstring &m_string_value)
{
	type = JSONType_String;
	string_value = m_string_value;
}

/**
 * Basic constructor for creating a JSON Value of type Bool
 *
 * @access public
 *
 * @param bool m_bool_value The bool to use as the value
 */
JSONValue::JSONValue(bool m_bool_value)
{
	type = JSONType_Bool;
	bool_value = m_bool_value;
}

/**
 * Basic constructor for creating a JSON Value of type Number
 *
 * @access public
 *
 * @param double m_number_value The number to use as the value
 */
JSONValue::JSONValue(double m_number_value)
{
	type = JSONType_Number;
	number_value = m_number_value;
}

/**
 * Basic constructor for creating a JSON Value of type Array
 *
 * @access public
 *
 * @param JSONArray m_array_value The JSONArray to use as the value
 */
JSONValue::JSONValue(const JSONArray &m_array_value)
{
	type = JSONType_Array;
	array_value = m_array_value;
}

/**
 * Basic constructor for creating a JSON Value of type Object
 *
 * @access public
 *
 * @param JSONObject m_object_value The JSONObject to use as the value
 */
JSONValue::JSONValue(const JSONObject &m_object_value)
{
	type = JSONType_Object;
	object_value = m_object_value;
}

/**
 * The destructor for the JSON Value object
 * Handles deleting the objects in the array or the object value
 *
 * @access public
 */
JSONValue::~JSONValue()
{
	if (type == JSONType_Array)
	{
		JSONArray::iterator iter;
		for (iter = array_value.begin(); iter != array_value.end(); iter++)
			delete *iter;
	}
	else if (type == JSONType_Object)
	{
		JSONObject::iterator iter;
		for (iter = object_value.begin(); iter != object_value.end(); iter++)
		{
			delete (*iter).second;
		}
	}
}

/**
 * Checks if the value is a NULL
 *
 * @access public
 *
 * @return bool Returns true if it is a NULL value, false otherwise
 */
bool JSONValue::IsNull() const
{
	return type == JSONType_Null;
}

/**
 * Checks if the value is a String
 *
 * @access public
 *
 * @return bool Returns true if it is a String value, false otherwise
 */
bool JSONValue::IsString() const
{
	return type == JSONType_String;
}

/**
 * Checks if the value is a Bool
 *
 * @access public
 *
 * @return bool Returns true if it is a Bool value, false otherwise
 */
bool JSONValue::IsBool() const
{
	return type == JSONType_Bool;
}

/**
 * Checks if the value is a Number
 *
 * @access public
 *
 * @return bool Returns true if it is a Number value, false otherwise
 */
bool JSONValue::IsNumber() const
{
	return type == JSONType_Number;
}

/**
 * Checks if the value is an Array
 *
 * @access public
 *
 * @return bool Returns true if it is an Array value, false otherwise
 */
bool JSONValue::IsArray() const
{
	return type == JSONType_Array;
}

/**
 * Checks if the value is an Object
 *
 * @access public
 *
 * @return bool Returns true if it is an Object value, false otherwise
 */
bool JSONValue::IsObject() const
{
	return type == JSONType_Object;
}

/**
 * Retrieves the String value of this JSONValue
 * Use IsString() before using this method.
 *
 * @access public
 *
 * @return std::wstring Returns the string value
 */
const std::wstring &JSONValue::AsString() const
{
	return string_value;
}

/**
 * Retrieves the Bool value of this JSONValue
 * Use IsBool() before using this method.
 *
 * @access public
 *
 * @return bool Returns the bool value
 */
bool JSONValue::AsBool() const
{
	return bool_value;
}

/**
 * Retrieves the Number value of this JSONValue
 * Use IsNumber() before using this method.
 *
 * @access public
 *
 * @return double Returns the number value
 */
double JSONValue::AsNumber() const
{
	return number_value;
}

/**
 * Retrieves the Array value of this JSONValue
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return JSONArray Returns the array value
 */
const JSONArray &JSONValue::AsArray() const
{
	return array_value;
}

/**
 * Retrieves the Object value of this JSONValue
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return JSONObject Returns the object value
 */
const JSONObject &JSONValue::AsObject() const
{
	return object_value;
}

/**
 * Retrieves the number of children of this JSONValue.
 * This number will be 0 or the actual number of children
 * if IsArray() or IsObject().
 *
 * @access public
 *
 * @return The number of children.
 */
std::size_t JSONValue::CountChildren() const
{
	switch (type)
	{
		case JSONType_Array:
			return array_value.size();
		case JSONType_Object:
			return object_value.size();
		default:
			return 0;
	}
}

/**
 * Checks if this JSONValue has a child at the given index.
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return bool Returns true if the array has a value at the given index.
 */
bool JSONValue::HasChild(std::size_t index) const
{
	if (type == JSONType_Array)
	{
		return index < array_value.size();
	}
	else
	{
		return false;
	}
}

/**
 * Retrieves the child of this JSONValue at the given index.
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return JSONValue* Returns JSONValue at the given index or NULL
 *                    if it doesn't exist.
 */
JSONValue *JSONValue::Child(std::size_t index)
{
	if (index < array_value.size())
	{
		return array_value[index];
	}
	else
	{
		return NULL;
	}
}

/**
 * Checks if this JSONValue has a child at the given key.
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return bool Returns true if the object has a value at the given key.
 */
bool JSONValue::HasChild(const wchar_t* name) const
{
	if (type == JSONType_Object)
	{
		return object_value.find(name) != object_value.end();
	}
	else
	{
		return false;
	}
}

/**
 * Retrieves the child of this JSONValue at the given key.
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return JSONValue* Returns JSONValue for the given key in the object
 *                    or NULL if it doesn't exist.
 */
JSONValue* JSONValue::Child(const wchar_t* name)
{
	JSONObject::const_iterator it = object_value.find(name);
	if (it != object_value.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

/**
 * Creates a JSON encoded string for the value with all necessary characters escaped
 *
 * @access public
 *
 * @return std::wstring Returns the JSON string
 */
std::wstring JSONValue::Stringify() const
{
	std::wstring ret_string;
	
	switch (type)
	{
		case JSONType_Null:
			ret_string = L"null";
			break;
		
		case JSONType_String:
			ret_string = StringifyString(string_value);
			break;
		
		case JSONType_Bool:
			ret_string = bool_value ? L"true" : L"false";
			break;
		
		case JSONType_Number:
		{
			if (isinf(number_value) || isnan(number_value))
				ret_string = L"null";
			else
			{
				std::wstringstream ss;
				ss.precision(15);
				ss << number_value;
				ret_string = ss.str();
			}
			break;
		}
		
		case JSONType_Array:
		{
			ret_string = L"[";
			JSONArray::const_iterator iter = array_value.begin();
			while (iter != array_value.end())
			{
				ret_string += (*iter)->Stringify();
				
				// Not at the end - add a separator
				if (++iter != array_value.end())
					ret_string += L",";
			}
			ret_string += L"]";
			break;
		}
		
		case JSONType_Object:
		{
			ret_string = L"{";
			JSONObject::const_iterator iter = object_value.begin();
			while (iter != object_value.end())
			{
				ret_string += StringifyString((*iter).first);
				ret_string += L":";
				ret_string += (*iter).second->Stringify();
				
				// Not at the end - add a separator
				if (++iter != object_value.end())
					ret_string += L",";
			}
			ret_string += L"}";
			break;
		}
	}

	return ret_string;
}

/**
 * Creates a JSON encoded string with all required fields escaped
 * Works from http://www.ecma-internationl.org/publications/files/ECMA-ST/ECMA-262.pdf
 * Section 15.12.3.
 *
 * @access private
 *
 * @param std::wstring str The string that needs to have the characters escaped
 *
 * @return std::wstring Returns the JSON string
 */
std::wstring JSONValue::StringifyString(const std::wstring &str)
{
	std::wstring str_out = L"\"";
	
	std::wstring::const_iterator iter = str.begin();
	while (iter != str.end())
	{
		wchar_t chr = *iter;

		if (chr == L'"' || chr == L'\\' || chr == L'/')
		{
			str_out += L'\\';
			str_out += chr;
		}
		else if (chr == L'\b')
		{
			str_out += L"\\b";
		}
		else if (chr == L'\f')
		{
			str_out += L"\\f";
		}
		else if (chr == L'\n')
		{
			str_out += L"\\n";
		}
		else if (chr == L'\r')
		{
			str_out += L"\\r";
		}
		else if (chr == L'\t')
		{
			str_out += L"\\t";
		}
		else if (chr < L' ')
		{
			str_out += L"\\u";
			for (int i = 0; i < 4; i++)
			{
				int value = (chr >> 12) & 0xf;
				if (value >= 0 && value <= 9)
					str_out += (wchar_t)('0' + value);
				else if (value >= 10 && value <= 15)
					str_out += (wchar_t)('A' + (value - 10));
				chr <<= 4;
			}
		}
		else
		{
			str_out += chr;
		}
		
		iter++;
	}
	
	str_out += L"\"";
	return str_out;
}



JSON::JSON()
{
}

/**
 * Parses a complete JSON encoded string
 * This is just a wrapper around the UNICODE Parse().
 *
 * @access public
 *
 * @param char* data The JSON text
 *
 * @return JSONValue* Returns a JSON Value representing the root, or NULL on error
 */

JSONValue *JSON::Parse(const char *data)
{
	size_t length = strlen(data) + 1;
	wchar_t *w_data = (wchar_t*)malloc(length * sizeof(wchar_t));
	
	#if defined(WIN32) && !defined(__GNUC__)
		size_t ret_value = 0;
		if (mbstowcs_s(&ret_value, w_data, length, data, length) != 0)
		{
			free(w_data);
			return NULL;
		}
	#else
		if (mbstowcs(w_data, data, length) == (size_t)-1)
		{
			free(w_data);
			return NULL;
		}
	#endif
	
	JSONValue *value = JSON::Parse(w_data);
	free(w_data);
	return value;
}

/**
 * Parses a complete JSON encoded string (UNICODE input version)
 *
 * @access public
 *
 * @param wchar_t* data The JSON text
 *
 * @return JSONValue* Returns a JSON Value representing the root, or NULL on error
 */
JSONValue *JSON::Parse(const wchar_t *data)
{
	// Skip any preceding whitespace, end of data = no JSON = fail
	if (!SkipWhitespace(&data))
		return NULL;

	// We need the start of a value here now...
	JSONValue *value = JSONValue::Parse(&data);
	if (value == NULL)
		return NULL;
	
	// Can be white space now and should be at the end of the string then...
	if (SkipWhitespace(&data))
	{
		delete value;
		return NULL;
	}
	
	// We're now at the end of the string
	return value;
}

/**
 * Turns the passed in JSONValue into a JSON encode string
 *
 * @access public
 *
 * @param JSONValue* value The root value
 *
 * @return std::wstring Returns a JSON encoded string representation of the given value
 */
std::wstring JSON::Stringify(const JSONValue *value)
{
	if (value != NULL)
		return value->Stringify();
	else
		return L"";
}

/**
 * Skips over any whitespace characters (space, tab, \r or \n) defined by the JSON spec
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return bool Returns true if there is more data, or false if the end of the text was reached
 */
bool JSON::SkipWhitespace(const wchar_t **data)
{
	while (**data != 0 && (**data == L' ' || **data == L'\t' || **data == L'\r' || **data == L'\n'))
		(*data)++;
	
	return **data != 0;
}

/**
 * Extracts a JSON String as defined by the spec - "<some chars>"
 * Any escaped characters are swapped out for their unescaped values
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 * @param std::wstring& str Reference to a std::wstring to receive the extracted string
 *
 * @return bool Returns true on success, false on failure
 */
bool JSON::ExtractString(const wchar_t **data, std::wstring &str)
{
	str = L"";
	
	while (**data != 0)
	{
		// Save the char so we can change it if need be
		wchar_t next_char = **data;
		
		// Escaping something?
		if (next_char == L'\\')
		{
			// Move over the escape char
			(*data)++;
			
			// Deal with the escaped char
			switch (**data)
			{
				case L'"': next_char = L'"'; break;
				case L'\\': next_char = L'\\'; break;
				case L'/': next_char = L'/'; break;
				case L'b': next_char = L'\b'; break;
				case L'f': next_char = L'\f'; break;
				case L'n': next_char = L'\n'; break;
				case L'r': next_char = L'\r'; break;
				case L't': next_char = L'\t'; break;
				case L'u':
				{
					// We need 5 chars (4 hex + the 'u') or its not valid
					if (!simplejson_wcsnlen(*data, 5))
						return false;
					
					// Deal with the chars
					next_char = 0;
					for (int i = 0; i < 4; i++)
					{
						// Do it first to move off the 'u' and leave us on the
						// final hex digit as we move on by one later on
						(*data)++;
						
						next_char <<= 4;
						
						// Parse the hex digit
						if (**data >= '0' && **data <= '9')
							next_char |= (**data - '0');
						else if (**data >= 'A' && **data <= 'F')
							next_char |= (10 + (**data - 'A'));
						else if (**data >= 'a' && **data <= 'f')
							next_char |= (10 + (**data - 'a'));
						else
						{
							// Invalid hex digit = invalid JSON
							return false;
						}
					}
					break;
				}
				
				// By the spec, only the above cases are allowed
				default:
					return false;
			}
		}
		
		// End of the string?
		else if (next_char == L'"')
		{
			(*data)++;
			str.reserve(); // Remove unused capacity
			return true;
		}
		
		// Disallowed char?
		else if (next_char < L' ' && next_char != L'\t')
		{
			// SPEC Violation: Allow tabs due to real world cases
			return false;
		}
		
		// Add the next char
		str += next_char;
		
		// Move on
		(*data)++;
	}
	
	// If we're here, the string ended incorrectly
	return false;
}

/**
 * Parses some text as though it is an integer
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return double Returns the double value of the number found
 */
double JSON::ParseInt(const wchar_t **data)
{
	double integer = 0;
	while (**data != 0 && **data >= '0' && **data <= '9')
		integer = integer * 10 + (*(*data)++ - '0');
	
	return integer;
}

/**
 * Parses some text as though it is a decimal
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return double Returns the double value of the decimal found
 */
double JSON::ParseDecimal(const wchar_t **data)
{
	double decimal = 0.0;
  double factor = 0.1;
	while (**data != 0 && **data >= '0' && **data <= '9')
  {
    int digit = (*(*data)++ - '0');
		decimal = decimal + digit * factor;
    factor *= 0.1;
  }
	return decimal;
}




 
/*
LodePNG version 20130831

Copyright (c) 2005-2013 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef LODEPNG_H
#define LODEPNG_H



/*
The following #defines are used to create code sections. They can be disabled
to disable code sections, which can give faster compile time and smaller binary.
The "NO_COMPILE" defines are designed to be used to pass as defines to the
compiler command to disable them without modifying this header, e.g.
-DLODEPNG_NO_COMPILE_ZLIB for gcc.
*/
/*deflate & zlib. If disabled, you must specify alternative zlib functions in
the custom_zlib field of the compress and decompress settings*/
#ifndef LODEPNG_NO_COMPILE_ZLIB
#define LODEPNG_COMPILE_ZLIB
#endif
/*png encoder and png decoder*/
#ifndef LODEPNG_NO_COMPILE_PNG
#define LODEPNG_COMPILE_PNG
#endif
/*deflate&zlib decoder and png decoder*/
#ifndef LODEPNG_NO_COMPILE_DECODER
#define LODEPNG_COMPILE_DECODER
#endif
/*deflate&zlib encoder and png encoder*/
#ifndef LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_COMPILE_ENCODER
#endif
/*the optional built in harddisk file loading and saving functions*/
#ifndef LODEPNG_NO_COMPILE_DISK
#define LODEPNG_COMPILE_DISK
#endif
/*support for chunks other than IHDR, IDAT, PLTE, tRNS, IEND: ancillary and unknown chunks*/
#ifndef LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_COMPILE_ANCILLARY_CHUNKS
#endif
/*ability to convert error numerical codes to English text string*/
#ifndef LODEPNG_NO_COMPILE_ERROR_TEXT
#define LODEPNG_COMPILE_ERROR_TEXT
#endif
/*Compile the default allocators (C's free, malloc and realloc). If you disable this,
you can define the functions lodepng_free, lodepng_malloc and lodepng_realloc in your
source files with custom allocators.*/
#ifndef LODEPNG_NO_COMPILE_ALLOCATORS
#define LODEPNG_COMPILE_ALLOCATORS
#endif
/*compile the C++ version (you can disable the C++ wrapper here even when compiling for C++)*/
#ifdef __cplusplus
#ifndef LODEPNG_NO_COMPILE_CPP
#define LODEPNG_COMPILE_CPP
#endif
#endif

#ifdef LODEPNG_COMPILE_PNG
/*The PNG color types (also used for raw).*/
typedef enum LodePNGColorType
{
  LCT_GREY = 0, /*greyscale: 1,2,4,8,16 bit*/
  LCT_RGB = 2, /*RGB: 8,16 bit*/
  LCT_PALETTE = 3, /*palette: 1,2,4,8 bit*/
  LCT_GREY_ALPHA = 4, /*greyscale with alpha: 8,16 bit*/
  LCT_RGBA = 6 /*RGB with alpha: 8,16 bit*/
} LodePNGColorType;

#ifdef LODEPNG_COMPILE_DECODER
/*
Converts PNG data in memory to raw pixel data.
out: Output parameter. Pointer to buffer that will contain the raw pixel data.
     After decoding, its size is w * h * (bytes per pixel) bytes larger than
     initially. Bytes per pixel depends on colortype and bitdepth.
     Must be freed after usage with free(*out).
     Note: for 16-bit per channel colors, uses big endian format like PNG does.
w: Output parameter. Pointer to width of pixel data.
h: Output parameter. Pointer to height of pixel data.
in: Memory buffer with the PNG file.
insize: size of the in buffer.
colortype: the desired color type for the raw output image. See explanation on PNG color types.
bitdepth: the desired bit depth for the raw output image. See explanation on PNG color types.
Return value: LodePNG error code (0 means no error).
*/
unsigned lodepng_decode_memory(unsigned char** out, unsigned* w, unsigned* h,
                               const unsigned char* in, size_t insize,
                               LodePNGColorType colortype, unsigned bitdepth);

/*Same as lodepng_decode_memory, but always decodes to 32-bit RGBA raw image*/
unsigned lodepng_decode32(unsigned char** out, unsigned* w, unsigned* h,
                          const unsigned char* in, size_t insize);

/*Same as lodepng_decode_memory, but always decodes to 24-bit RGB raw image*/
unsigned lodepng_decode24(unsigned char** out, unsigned* w, unsigned* h,
                          const unsigned char* in, size_t insize);

#ifdef LODEPNG_COMPILE_DISK
/*
Load PNG from disk, from file with given name.
Same as the other decode functions, but instead takes a filename as input.
*/
unsigned lodepng_decode_file(unsigned char** out, unsigned* w, unsigned* h,
                             const char* filename,
                             LodePNGColorType colortype, unsigned bitdepth);

/*Same as lodepng_decode_file, but always decodes to 32-bit RGBA raw image.*/
unsigned lodepng_decode32_file(unsigned char** out, unsigned* w, unsigned* h,
                               const char* filename);

/*Same as lodepng_decode_file, but always decodes to 24-bit RGB raw image.*/
unsigned lodepng_decode24_file(unsigned char** out, unsigned* w, unsigned* h,
                               const char* filename);
#endif /*LODEPNG_COMPILE_DISK*/
#endif /*LODEPNG_COMPILE_DECODER*/


#ifdef LODEPNG_COMPILE_ENCODER
/*
Converts raw pixel data into a PNG image in memory. The colortype and bitdepth
  of the output PNG image cannot be chosen, they are automatically determined
  by the colortype, bitdepth and content of the input pixel data.
  Note: for 16-bit per channel colors, needs big endian format like PNG does.
out: Output parameter. Pointer to buffer that will contain the PNG image data.
     Must be freed after usage with free(*out).
outsize: Output parameter. Pointer to the size in bytes of the out buffer.
image: The raw pixel data to encode. The size of this buffer should be
       w * h * (bytes per pixel), bytes per pixel depends on colortype and bitdepth.
w: width of the raw pixel data in pixels.
h: height of the raw pixel data in pixels.
colortype: the color type of the raw input image. See explanation on PNG color types.
bitdepth: the bit depth of the raw input image. See explanation on PNG color types.
Return value: LodePNG error code (0 means no error).
*/
unsigned lodepng_encode_memory(unsigned char** out, size_t* outsize,
                               const unsigned char* image, unsigned w, unsigned h,
                               LodePNGColorType colortype, unsigned bitdepth);

/*Same as lodepng_encode_memory, but always encodes from 32-bit RGBA raw image.*/
unsigned lodepng_encode32(unsigned char** out, size_t* outsize,
                          const unsigned char* image, unsigned w, unsigned h);

/*Same as lodepng_encode_memory, but always encodes from 24-bit RGB raw image.*/
unsigned lodepng_encode24(unsigned char** out, size_t* outsize,
                          const unsigned char* image, unsigned w, unsigned h);

#ifdef LODEPNG_COMPILE_DISK
/*
Converts raw pixel data into a PNG file on disk.
Same as the other encode functions, but instead takes a filename as output.
NOTE: This overwrites existing files without warning!
*/
unsigned lodepng_encode_file(const char* filename,
                             const unsigned char* image, unsigned w, unsigned h,
                             LodePNGColorType colortype, unsigned bitdepth);

/*Same as lodepng_encode_file, but always encodes from 32-bit RGBA raw image.*/
unsigned lodepng_encode32_file(const char* filename,
                               const unsigned char* image, unsigned w, unsigned h);

/*Same as lodepng_encode_file, but always encodes from 24-bit RGB raw image.*/
unsigned lodepng_encode24_file(const char* filename,
                               const unsigned char* image, unsigned w, unsigned h);
#endif /*LODEPNG_COMPILE_DISK*/
#endif /*LODEPNG_COMPILE_ENCODER*/


#ifdef LODEPNG_COMPILE_CPP
namespace lodepng
{
#ifdef LODEPNG_COMPILE_DECODER
/*Same as lodepng_decode_memory, but decodes to an std::vector.*/
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const unsigned char* in, size_t insize,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const std::vector<unsigned char>& in,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
#ifdef LODEPNG_COMPILE_DISK
/*
Converts PNG file from disk to raw pixel data in memory.
Same as the other decode functions, but instead takes a filename as input.
*/
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const std::string& filename,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
#endif //LODEPNG_COMPILE_DISK
#endif //LODEPNG_COMPILE_DECODER

#ifdef LODEPNG_COMPILE_ENCODER
/*Same as lodepng_encode_memory, but encodes to an std::vector.*/
unsigned encode(std::vector<unsigned char>& out,
                const unsigned char* in, unsigned w, unsigned h,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
unsigned encode(std::vector<unsigned char>& out,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
#ifdef LODEPNG_COMPILE_DISK
/*
Converts 32-bit RGBA raw pixel data into a PNG file on disk.
Same as the other encode functions, but instead takes a filename as output.
NOTE: This overwrites existing files without warning!
*/
unsigned encode(const std::string& filename,
                const unsigned char* in, unsigned w, unsigned h,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
unsigned encode(const std::string& filename,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
#endif //LODEPNG_COMPILE_DISK
#endif //LODEPNG_COMPILE_ENCODER
} //namespace lodepng
#endif /*LODEPNG_COMPILE_CPP*/
#endif /*LODEPNG_COMPILE_PNG*/

#ifdef LODEPNG_COMPILE_ERROR_TEXT
/*Returns an English description of the numerical error code.*/
const char* lodepng_error_text(unsigned code);
#endif /*LODEPNG_COMPILE_ERROR_TEXT*/

#ifdef LODEPNG_COMPILE_DECODER
/*Settings for zlib decompression*/
typedef struct LodePNGDecompressSettings LodePNGDecompressSettings;
struct LodePNGDecompressSettings
{
  unsigned ignore_adler32; /*if 1, continue and don't give an error message if the Adler32 checksum is corrupted*/

  /*use custom zlib decoder instead of built in one (default: null)*/
  unsigned (*custom_zlib)(unsigned char**, size_t*,
                          const unsigned char*, size_t,
                          const LodePNGDecompressSettings*);
  /*use custom deflate decoder instead of built in one (default: null)
  if custom_zlib is used, custom_deflate is ignored since only the built in
  zlib function will call custom_deflate*/
  unsigned (*custom_inflate)(unsigned char**, size_t*,
                             const unsigned char*, size_t,
                             const LodePNGDecompressSettings*);

  void* custom_context; /*optional custom settings for custom functions*/
};

extern const LodePNGDecompressSettings lodepng_default_decompress_settings;
void lodepng_decompress_settings_init(LodePNGDecompressSettings* settings);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
/*
Settings for zlib compression. Tweaking these settings tweaks the balance
between speed and compression ratio.
*/
typedef struct LodePNGCompressSettings LodePNGCompressSettings;
struct LodePNGCompressSettings /*deflate = compress*/
{
  /*LZ77 related settings*/
  unsigned btype; /*the block type for LZ (0, 1, 2 or 3, see zlib standard). Should be 2 for proper compression.*/
  unsigned use_lz77; /*whether or not to use LZ77. Should be 1 for proper compression.*/
  unsigned windowsize; /*the maximum is 32768, higher gives more compression but is slower. Typical value: 2048.*/
  unsigned minmatch; /*mininum lz77 length. 3 is normally best, 6 can be better for some PNGs. Default: 0*/
  unsigned nicematch; /*stop searching if >= this length found. Set to 258 for best compression. Default: 128*/
  unsigned lazymatching; /*use lazy matching: better compression but a bit slower. Default: true*/

  /*use custom zlib encoder instead of built in one (default: null)*/
  unsigned (*custom_zlib)(unsigned char**, size_t*,
                          const unsigned char*, size_t,
                          const LodePNGCompressSettings*);
  /*use custom deflate encoder instead of built in one (default: null)
  if custom_zlib is used, custom_deflate is ignored since only the built in
  zlib function will call custom_deflate*/
  unsigned (*custom_deflate)(unsigned char**, size_t*,
                             const unsigned char*, size_t,
                             const LodePNGCompressSettings*);

  void* custom_context; /*optional custom settings for custom functions*/
};

extern const LodePNGCompressSettings lodepng_default_compress_settings;
void lodepng_compress_settings_init(LodePNGCompressSettings* settings);
#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_PNG
/*
Color mode of an image. Contains all information required to decode the pixel
bits to RGBA colors. This information is the same as used in the PNG file
format, and is used both for PNG and raw image data in LodePNG.
*/
typedef struct LodePNGColorMode
{
  /*header (IHDR)*/
  LodePNGColorType colortype; /*color type, see PNG standard or documentation further in this header file*/
  unsigned bitdepth;  /*bits per sample, see PNG standard or documentation further in this header file*/

  /*
  palette (PLTE and tRNS)

  Dynamically allocated with the colors of the palette, including alpha.
  When encoding a PNG, to store your colors in the palette of the LodePNGColorMode, first use
  lodepng_palette_clear, then for each color use lodepng_palette_add.
  If you encode an image without alpha with palette, don't forget to put value 255 in each A byte of the palette.

  When decoding, by default you can ignore this palette, since LodePNG already
  fills the palette colors in the pixels of the raw RGBA output.

  The palette is only supported for color type 3.
  */
  unsigned char* palette; /*palette in RGBARGBA... order. When allocated, must be either 0, or have size 1024*/
  size_t palettesize; /*palette size in number of colors (amount of bytes is 4 * palettesize)*/

  /*
  transparent color key (tRNS)

  This color uses the same bit depth as the bitdepth value in this struct, which can be 1-bit to 16-bit.
  For greyscale PNGs, r, g and b will all 3 be set to the same.

  When decoding, by default you can ignore this information, since LodePNG sets
  pixels with this key to transparent already in the raw RGBA output.

  The color key is only supported for color types 0 and 2.
  */
  unsigned key_defined; /*is a transparent color key given? 0 = false, 1 = true*/
  unsigned key_r;       /*red/greyscale component of color key*/
  unsigned key_g;       /*green component of color key*/
  unsigned key_b;       /*blue component of color key*/
} LodePNGColorMode;

/*init, cleanup and copy functions to use with this struct*/
void lodepng_color_mode_init(LodePNGColorMode* info);
void lodepng_color_mode_cleanup(LodePNGColorMode* info);
/*return value is error code (0 means no error)*/
unsigned lodepng_color_mode_copy(LodePNGColorMode* dest, const LodePNGColorMode* source);

void lodepng_palette_clear(LodePNGColorMode* info);
/*add 1 color to the palette*/
unsigned lodepng_palette_add(LodePNGColorMode* info,
                             unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/*get the total amount of bits per pixel, based on colortype and bitdepth in the struct*/
unsigned lodepng_get_bpp(const LodePNGColorMode* info);
/*get the amount of color channels used, based on colortype in the struct.
If a palette is used, it counts as 1 channel.*/
unsigned lodepng_get_channels(const LodePNGColorMode* info);
/*is it a greyscale type? (only colortype 0 or 4)*/
unsigned lodepng_is_greyscale_type(const LodePNGColorMode* info);
/*has it got an alpha channel? (only colortype 2 or 6)*/
unsigned lodepng_is_alpha_type(const LodePNGColorMode* info);
/*has it got a palette? (only colortype 3)*/
unsigned lodepng_is_palette_type(const LodePNGColorMode* info);
/*only returns true if there is a palette and there is a value in the palette with alpha < 255.
Loops through the palette to check this.*/
unsigned lodepng_has_palette_alpha(const LodePNGColorMode* info);
/*
Check if the given color info indicates the possibility of having non-opaque pixels in the PNG image.
Returns true if the image can have translucent or invisible pixels (it still be opaque if it doesn't use such pixels).
Returns false if the image can only have opaque pixels.
In detail, it returns true only if it's a color type with alpha, or has a palette with non-opaque values,
or if "key_defined" is true.
*/
unsigned lodepng_can_have_alpha(const LodePNGColorMode* info);
/*Returns the byte size of a raw image buffer with given width, height and color mode*/
size_t lodepng_get_raw_size(unsigned w, unsigned h, const LodePNGColorMode* color);

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
/*The information of a Time chunk in PNG.*/
typedef struct LodePNGTime
{
  unsigned year;    /*2 bytes used (0-65535)*/
  unsigned month;   /*1-12*/
  unsigned day;     /*1-31*/
  unsigned hour;    /*0-23*/
  unsigned minute;  /*0-59*/
  unsigned second;  /*0-60 (to allow for leap seconds)*/
} LodePNGTime;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

/*Information about the PNG image, except pixels, width and height.*/
typedef struct LodePNGInfo
{
  /*header (IHDR), palette (PLTE) and transparency (tRNS) chunks*/
  unsigned compression_method;/*compression method of the original file. Always 0.*/
  unsigned filter_method;     /*filter method of the original file*/
  unsigned interlace_method;  /*interlace method of the original file*/
  LodePNGColorMode color;     /*color type and bits, palette and transparency of the PNG file*/

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  /*
  suggested background color chunk (bKGD)
  This color uses the same color mode as the PNG (except alpha channel), which can be 1-bit to 16-bit.

  For greyscale PNGs, r, g and b will all 3 be set to the same. When encoding
  the encoder writes the red one. For palette PNGs: When decoding, the RGB value
  will be stored, not a palette index. But when encoding, specify the index of
  the palette in background_r, the other two are then ignored.

  The decoder does not use this background color to edit the color of pixels.
  */
  unsigned background_defined; /*is a suggested background color given?*/
  unsigned background_r;       /*red component of suggested background color*/
  unsigned background_g;       /*green component of suggested background color*/
  unsigned background_b;       /*blue component of suggested background color*/

  /*
  non-international text chunks (tEXt and zTXt)

  The char** arrays each contain num strings. The actual messages are in
  text_strings, while text_keys are keywords that give a short description what
  the actual text represents, e.g. Title, Author, Description, or anything else.

  A keyword is minimum 1 character and maximum 79 characters long. It's
  discouraged to use a single line length longer than 79 characters for texts.

  Don't allocate these text buffers yourself. Use the init/cleanup functions
  correctly and use lodepng_add_text and lodepng_clear_text.
  */
  size_t text_num; /*the amount of texts in these char** buffers (there may be more texts in itext)*/
  char** text_keys; /*the keyword of a text chunk (e.g. "Comment")*/
  char** text_strings; /*the actual text*/

  /*
  international text chunks (iTXt)
  Similar to the non-international text chunks, but with additional strings
  "langtags" and "transkeys".
  */
  size_t itext_num; /*the amount of international texts in this PNG*/
  char** itext_keys; /*the English keyword of the text chunk (e.g. "Comment")*/
  char** itext_langtags; /*language tag for this text's language, ISO/IEC 646 string, e.g. ISO 639 language tag*/
  char** itext_transkeys; /*keyword translated to the international language - UTF-8 string*/
  char** itext_strings; /*the actual international text - UTF-8 string*/

  /*time chunk (tIME)*/
  unsigned time_defined; /*set to 1 to make the encoder generate a tIME chunk*/
  LodePNGTime time;

  /*phys chunk (pHYs)*/
  unsigned phys_defined; /*if 0, there is no pHYs chunk and the values below are undefined, if 1 else there is one*/
  unsigned phys_x; /*pixels per unit in x direction*/
  unsigned phys_y; /*pixels per unit in y direction*/
  unsigned phys_unit; /*may be 0 (unknown unit) or 1 (metre)*/

  /*
  unknown chunks
  There are 3 buffers, one for each position in the PNG where unknown chunks can appear
  each buffer contains all unknown chunks for that position consecutively
  The 3 buffers are the unknown chunks between certain critical chunks:
  0: IHDR-PLTE, 1: PLTE-IDAT, 2: IDAT-IEND
  Do not allocate or traverse this data yourself. Use the chunk traversing functions declared
  later, such as lodepng_chunk_next and lodepng_chunk_append, to read/write this struct.
  */
  unsigned char* unknown_chunks_data[3];
  size_t unknown_chunks_size[3]; /*size in bytes of the unknown chunks, given for protection*/
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGInfo;

/*init, cleanup and copy functions to use with this struct*/
void lodepng_info_init(LodePNGInfo* info);
void lodepng_info_cleanup(LodePNGInfo* info);
/*return value is error code (0 means no error)*/
unsigned lodepng_info_copy(LodePNGInfo* dest, const LodePNGInfo* source);

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
void lodepng_clear_text(LodePNGInfo* info); /*use this to clear the texts again after you filled them in*/
unsigned lodepng_add_text(LodePNGInfo* info, const char* key, const char* str); /*push back both texts at once*/

void lodepng_clear_itext(LodePNGInfo* info); /*use this to clear the itexts again after you filled them in*/
unsigned lodepng_add_itext(LodePNGInfo* info, const char* key, const char* langtag,
                           const char* transkey, const char* str); /*push back the 4 texts of 1 chunk at once*/
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

/*
Converts raw buffer from one color type to another color type, based on
LodePNGColorMode structs to describe the input and output color type.
See the reference manual at the end of this header file to see which color conversions are supported.
return value = LodePNG error code (0 if all went ok, an error if the conversion isn't supported)
The out buffer must have size (w * h * bpp + 7) / 8, where bpp is the bits per pixel
of the output color type (lodepng_get_bpp)
The fix_png value works as described in struct LodePNGDecoderSettings.
Note: for 16-bit per channel colors, uses big endian format like PNG does.
*/
unsigned lodepng_convert(unsigned char* out, const unsigned char* in,
                         LodePNGColorMode* mode_out, LodePNGColorMode* mode_in,
                         unsigned w, unsigned h, unsigned fix_png);


#ifdef LODEPNG_COMPILE_DECODER
/*
Settings for the decoder. This contains settings for the PNG and the Zlib
decoder, but not the Info settings from the Info structs.
*/
typedef struct LodePNGDecoderSettings
{
  LodePNGDecompressSettings zlibsettings; /*in here is the setting to ignore Adler32 checksums*/

  unsigned ignore_crc; /*ignore CRC checksums*/
  /*
  The fix_png setting, if 1, makes the decoder tolerant towards some PNG images
  that do not correctly follow the PNG specification. This only supports errors
  that are fixable, were found in images that are actually used on the web, and
  are silently tolerated by other decoders as well. Currently only one such fix
  is implemented: if a palette index is out of bounds given the palette size,
  interpret it as opaque black.
  By default this value is 0, which makes it stop with an error on such images.
  */
  unsigned fix_png;
  unsigned color_convert; /*whether to convert the PNG to the color type you want. Default: yes*/

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  unsigned read_text_chunks; /*if false but remember_unknown_chunks is true, they're stored in the unknown chunks*/
  /*store all bytes from unknown chunks in the LodePNGInfo (off by default, useful for a png editor)*/
  unsigned remember_unknown_chunks;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGDecoderSettings;

void lodepng_decoder_settings_init(LodePNGDecoderSettings* settings);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
/*automatically use color type with less bits per pixel if losslessly possible. Default: AUTO*/
typedef enum LodePNGFilterStrategy
{
  /*every filter at zero*/
  LFS_ZERO,
  /*Use filter that gives minumum sum, as described in the official PNG filter heuristic.*/
  LFS_MINSUM,
  /*Use the filter type that gives smallest Shannon entropy for this scanline. Depending
  on the image, this is better or worse than minsum.*/
  LFS_ENTROPY,
  /*
  Brute-force-search PNG filters by compressing each filter for each scanline.
  Experimental, very slow, and only rarely gives better compression than MINSUM.
  */
  LFS_BRUTE_FORCE,
  /*use predefined_filters buffer: you specify the filter type for each scanline*/
  LFS_PREDEFINED
} LodePNGFilterStrategy;

/*automatically use color type with less bits per pixel if losslessly possible. Default: LAC_AUTO*/
typedef enum LodePNGAutoConvert
{
  LAC_NO, /*use color type user requested*/
  LAC_ALPHA, /*use color type user requested, but if only opaque pixels and RGBA or grey+alpha, use RGB or grey*/
  LAC_AUTO, /*use PNG color type that can losslessly represent the uncompressed image the smallest possible*/
  /*
  like AUTO, but do not choose 1, 2 or 4 bit per pixel types.
  sometimes a PNG image compresses worse if less than 8 bits per pixels.
  */
  LAC_AUTO_NO_NIBBLES,
  /*
  like AUTO, but never choose palette color type. For small images, encoding
  the palette may take more bytes than what is gained. Note that AUTO also
  already prevents encoding the palette for extremely small images, but that may
  not be sufficient because due to the compression it cannot predict when to
  switch.
  */
  LAC_AUTO_NO_PALETTE,
  LAC_AUTO_NO_NIBBLES_NO_PALETTE
} LodePNGAutoConvert;


/*Settings for the encoder.*/
typedef struct LodePNGEncoderSettings
{
  LodePNGCompressSettings zlibsettings; /*settings for the zlib encoder, such as window size, ...*/

  LodePNGAutoConvert auto_convert; /*how to automatically choose output PNG color type, if at all*/

  /*If true, follows the official PNG heuristic: if the PNG uses a palette or lower than
  8 bit depth, set all filters to zero. Otherwise use the filter_strategy. Note that to
  completely follow the official PNG heuristic, filter_palette_zero must be true and
  filter_strategy must be LFS_MINSUM*/
  unsigned filter_palette_zero;
  /*Which filter strategy to use when not using zeroes due to filter_palette_zero.
  Set filter_palette_zero to 0 to ensure always using your chosen strategy. Default: LFS_MINSUM*/
  LodePNGFilterStrategy filter_strategy;
  /*used if filter_strategy is LFS_PREDEFINED. In that case, this must point to a buffer with
  the same length as the amount of scanlines in the image, and each value must <= 5. You
  have to cleanup this buffer, LodePNG will never free it. Don't forget that filter_palette_zero
  must be set to 0 to ensure this is also used on palette or low bitdepth images.*/
  unsigned char* predefined_filters;

  /*force creating a PLTE chunk if colortype is 2 or 6 (= a suggested palette).
  If colortype is 3, PLTE is _always_ created.*/
  unsigned force_palette;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  /*add LodePNG identifier and version as a text chunk, for debugging*/
  unsigned add_id;
  /*encode text chunks as zTXt chunks instead of tEXt chunks, and use compression in iTXt chunks*/
  unsigned text_compression;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGEncoderSettings;

void lodepng_encoder_settings_init(LodePNGEncoderSettings* settings);
#endif /*LODEPNG_COMPILE_ENCODER*/


#if defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER)
/*The settings, state and information for extended encoding and decoding.*/
typedef struct LodePNGState
{
#ifdef LODEPNG_COMPILE_DECODER
  LodePNGDecoderSettings decoder; /*the decoding settings*/
#endif /*LODEPNG_COMPILE_DECODER*/
#ifdef LODEPNG_COMPILE_ENCODER
  LodePNGEncoderSettings encoder; /*the encoding settings*/
#endif /*LODEPNG_COMPILE_ENCODER*/
  LodePNGColorMode info_raw; /*specifies the format in which you would like to get the raw pixel buffer*/
  LodePNGInfo info_png; /*info of the PNG image obtained after decoding*/
  unsigned error;
#ifdef LODEPNG_COMPILE_CPP
  //For the lodepng::State subclass.
  virtual ~LodePNGState(){}
#endif
} LodePNGState;

/*init, cleanup and copy functions to use with this struct*/
void lodepng_state_init(LodePNGState* state);
void lodepng_state_cleanup(LodePNGState* state);
void lodepng_state_copy(LodePNGState* dest, const LodePNGState* source);
#endif /* defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER) */

#ifdef LODEPNG_COMPILE_DECODER
/*
Same as lodepng_decode_memory, but uses a LodePNGState to allow custom settings and
getting much more information about the PNG image and color mode.
*/
unsigned lodepng_decode(unsigned char** out, unsigned* w, unsigned* h,
                        LodePNGState* state,
                        const unsigned char* in, size_t insize);

/*
Read the PNG header, but not the actual data. This returns only the information
that is in the header chunk of the PNG, such as width, height and color type. The
information is placed in the info_png field of the LodePNGState.
*/
unsigned lodepng_inspect(unsigned* w, unsigned* h,
                         LodePNGState* state,
                         const unsigned char* in, size_t insize);
#endif /*LODEPNG_COMPILE_DECODER*/


#ifdef LODEPNG_COMPILE_ENCODER
/*This function allocates the out buffer with standard malloc and stores the size in *outsize.*/
unsigned lodepng_encode(unsigned char** out, size_t* outsize,
                        const unsigned char* image, unsigned w, unsigned h,
                        LodePNGState* state);
#endif /*LODEPNG_COMPILE_ENCODER*/

/*
The lodepng_chunk functions are normally not needed, except to traverse the
unknown chunks stored in the LodePNGInfo struct, or add new ones to it.
It also allows traversing the chunks of an encoded PNG file yourself.

PNG standard chunk naming conventions:
First byte: uppercase = critical, lowercase = ancillary
Second byte: uppercase = public, lowercase = private
Third byte: must be uppercase
Fourth byte: uppercase = unsafe to copy, lowercase = safe to copy
*/

/*get the length of the data of the chunk. Total chunk length has 12 bytes more.*/
unsigned lodepng_chunk_length(const unsigned char* chunk);

/*puts the 4-byte type in null terminated string*/
void lodepng_chunk_type(char type[5], const unsigned char* chunk);

/*check if the type is the given type*/
unsigned char lodepng_chunk_type_equals(const unsigned char* chunk, const char* type);

/*0: it's one of the critical chunk types, 1: it's an ancillary chunk (see PNG standard)*/
unsigned char lodepng_chunk_ancillary(const unsigned char* chunk);

/*0: public, 1: private (see PNG standard)*/
unsigned char lodepng_chunk_private(const unsigned char* chunk);

/*0: the chunk is unsafe to copy, 1: the chunk is safe to copy (see PNG standard)*/
unsigned char lodepng_chunk_safetocopy(const unsigned char* chunk);

/*get pointer to the data of the chunk, where the input points to the header of the chunk*/
unsigned char* lodepng_chunk_data(unsigned char* chunk);
const unsigned char* lodepng_chunk_data_const(const unsigned char* chunk);

/*returns 0 if the crc is correct, 1 if it's incorrect (0 for OK as usual!)*/
unsigned lodepng_chunk_check_crc(const unsigned char* chunk);

/*generates the correct CRC from the data and puts it in the last 4 bytes of the chunk*/
void lodepng_chunk_generate_crc(unsigned char* chunk);

/*iterate to next chunks. don't use on IEND chunk, as there is no next chunk then*/
unsigned char* lodepng_chunk_next(unsigned char* chunk);
const unsigned char* lodepng_chunk_next_const(const unsigned char* chunk);

/*
Appends chunk to the data in out. The given chunk should already have its chunk header.
The out variable and outlength are updated to reflect the new reallocated buffer.
Returns error code (0 if it went ok)
*/
unsigned lodepng_chunk_append(unsigned char** out, size_t* outlength, const unsigned char* chunk);

/*
Appends new chunk to out. The chunk to append is given by giving its length, type
and data separately. The type is a 4-letter string.
The out variable and outlength are updated to reflect the new reallocated buffer.
Returne error code (0 if it went ok)
*/
unsigned lodepng_chunk_create(unsigned char** out, size_t* outlength, unsigned length,
                              const char* type, const unsigned char* data);


/*Calculate CRC32 of buffer*/
unsigned lodepng_crc32(const unsigned char* buf, size_t len);
#endif /*LODEPNG_COMPILE_PNG*/


#ifdef LODEPNG_COMPILE_ZLIB
/*
This zlib part can be used independently to zlib compress and decompress a
buffer. It cannot be used to create gzip files however, and it only supports the
part of zlib that is required for PNG, it does not support dictionaries.
*/

#ifdef LODEPNG_COMPILE_DECODER
/*Inflate a buffer. Inflate is the decompression step of deflate. Out buffer must be freed after use.*/
unsigned lodepng_inflate(unsigned char** out, size_t* outsize,
                         const unsigned char* in, size_t insize,
                         const LodePNGDecompressSettings* settings);

/*
Decompresses Zlib data. Reallocates the out buffer and appends the data. The
data must be according to the zlib specification.
Either, *out must be NULL and *outsize must be 0, or, *out must be a valid
buffer and *outsize its size in bytes. out must be freed by user after usage.
*/
unsigned lodepng_zlib_decompress(unsigned char** out, size_t* outsize,
                                 const unsigned char* in, size_t insize,
                                 const LodePNGDecompressSettings* settings);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
/*
Compresses data with Zlib. Reallocates the out buffer and appends the data.
Zlib adds a small header and trailer around the deflate data.
The data is output in the format of the zlib specification.
Either, *out must be NULL and *outsize must be 0, or, *out must be a valid
buffer and *outsize its size in bytes. out must be freed by user after usage.
*/
unsigned lodepng_zlib_compress(unsigned char** out, size_t* outsize,
                               const unsigned char* in, size_t insize,
                               const LodePNGCompressSettings* settings);

/*
Find length-limited Huffman code for given frequencies. This function is in the
public interface only for tests, it's used internally by lodepng_deflate.
*/
unsigned lodepng_huffman_code_lengths(unsigned* lengths, const unsigned* frequencies,
                                      size_t numcodes, unsigned maxbitlen);

/*Compress a buffer with deflate. See RFC 1951. Out buffer must be freed after use.*/
unsigned lodepng_deflate(unsigned char** out, size_t* outsize,
                         const unsigned char* in, size_t insize,
                         const LodePNGCompressSettings* settings);

#endif /*LODEPNG_COMPILE_ENCODER*/
#endif /*LODEPNG_COMPILE_ZLIB*/

#ifdef LODEPNG_COMPILE_DISK
/*
Load a file from disk into buffer. The function allocates the out buffer, and
after usage you should free it.
out: output parameter, contains pointer to loaded buffer.
outsize: output parameter, size of the allocated out buffer
filename: the path to the file to load
return value: error code (0 means ok)
*/
unsigned lodepng_load_file(unsigned char** out, size_t* outsize, const char* filename);

/*
Save a file from buffer to disk. Warning, if it exists, this function overwrites
the file without warning!
buffer: the buffer to write
buffersize: size of the buffer to write
filename: the path to the file to save to
return value: error code (0 means ok)
*/
unsigned lodepng_save_file(const unsigned char* buffer, size_t buffersize, const char* filename);
#endif /*LODEPNG_COMPILE_DISK*/

#ifdef LODEPNG_COMPILE_CPP
//The LodePNG C++ wrapper uses std::vectors instead of manually allocated memory buffers.
namespace lodepng
{
#ifdef LODEPNG_COMPILE_PNG
class State : public LodePNGState
{
  public:
    State();
    State(const State& other);
    virtual ~State();
    State& operator=(const State& other);
};

#ifdef LODEPNG_COMPILE_DECODER
//Same as other lodepng::decode, but using a State for more settings and information.
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                State& state,
                const unsigned char* in, size_t insize);
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                State& state,
                const std::vector<unsigned char>& in);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
//Same as other lodepng::encode, but using a State for more settings and information.
unsigned encode(std::vector<unsigned char>& out,
                const unsigned char* in, unsigned w, unsigned h,
                State& state);
unsigned encode(std::vector<unsigned char>& out,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                State& state);
#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_DISK
/*
Load a file from disk into an std::vector. If the vector is empty, then either
the file doesn't exist or is an empty file.
*/
void load_file(std::vector<unsigned char>& buffer, const std::string& filename);

/*
Save the binary data in an std::vector to a file on disk. The file is overwritten
without warning.
*/
void save_file(const std::vector<unsigned char>& buffer, const std::string& filename);
#endif //LODEPNG_COMPILE_DISK
#endif //LODEPNG_COMPILE_PNG

#ifdef LODEPNG_COMPILE_ZLIB
#ifdef LODEPNG_COMPILE_DECODER
//Zlib-decompress an unsigned char buffer
unsigned decompress(std::vector<unsigned char>& out, const unsigned char* in, size_t insize,
                    const LodePNGDecompressSettings& settings = lodepng_default_decompress_settings);

//Zlib-decompress an std::vector
unsigned decompress(std::vector<unsigned char>& out, const std::vector<unsigned char>& in,
                    const LodePNGDecompressSettings& settings = lodepng_default_decompress_settings);
#endif //LODEPNG_COMPILE_DECODER

#ifdef LODEPNG_COMPILE_ENCODER
//Zlib-compress an unsigned char buffer
unsigned compress(std::vector<unsigned char>& out, const unsigned char* in, size_t insize,
                  const LodePNGCompressSettings& settings = lodepng_default_compress_settings);

//Zlib-compress an std::vector
unsigned compress(std::vector<unsigned char>& out, const std::vector<unsigned char>& in,
                  const LodePNGCompressSettings& settings = lodepng_default_compress_settings);
#endif //LODEPNG_COMPILE_ENCODER
#endif //LODEPNG_COMPILE_ZLIB
} //namespace lodepng
#endif /*LODEPNG_COMPILE_CPP*/

/*
TODO:
[.] test if there are no memory leaks or security exploits - done a lot but needs to be checked often
[.] check compatibility with vareous compilers  - done but needs to be redone for every newer version
[X] converting color to 16-bit per channel types
[ ] read all public PNG chunk types (but never let the color profile and gamma ones touch RGB values)
[ ] make sure encoder generates no chunks with size > (2^31)-1
[ ] partial decoding (stream processing)
[X] let the "isFullyOpaque" function check color keys and transparent palettes too
[X] better name for the variables "codes", "codesD", "codelengthcodes", "clcl" and "lldl"
[ ] don't stop decoding on errors like 69, 57, 58 (make warnings)
[ ] make option to choose if the raw image with non multiple of 8 bits per scanline should have padding bits or not
[ ] let the C++ wrapper catch exceptions coming from the standard library and return LodePNG error codes
*/

#endif /*LODEPNG_H inclusion guard*/

/*
LodePNG version 20130831

Copyright (c) 2005-2013 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

/*
The manual and changelog are in the header file "lodepng.h"
Rename this file to lodepng.cpp to use it for C++, or to lodepng.c to use it for C.
*/


#ifdef LODEPNG_COMPILE_CPP

#endif /*LODEPNG_COMPILE_CPP*/

#define VERSION_STRING "20130831"

/*
This source file is built up in the following large parts. The code sections
with the "LODEPNG_COMPILE_" #defines divide this up further in an intermixed way.
-Tools for C and common code for PNG and Zlib
-C Code for Zlib (huffman, deflate, ...)
-C Code for PNG (file format chunks, adam7, PNG filters, color conversions, ...)
-The C++ wrapper around all of the above
*/

/*The malloc, realloc and free functions defined here with "lodepng_" in front
of the name, so that you can easily change them to others related to your
platform if needed. Everything else in the code calls these. Pass
-DLODEPNG_NO_COMPILE_ALLOCATORS to the compiler, or comment out
#define LODEPNG_COMPILE_ALLOCATORS in the header, to disable the ones here and
define them in your own project's source files without needing to change
lodepng source code. Don't forget to remove "static" if you copypaste them
from here.*/

#ifdef LODEPNG_COMPILE_ALLOCATORS
static void* lodepng_malloc(size_t size)
{
  return malloc(size);
}

static void* lodepng_realloc(void* ptr, size_t new_size)
{
  return realloc(ptr, new_size);
}

static void lodepng_free(void* ptr)
{
  free(ptr);
}
#else /*LODEPNG_COMPILE_ALLOCATORS*/
void* lodepng_malloc(size_t size);
void* lodepng_realloc(void* ptr, size_t new_size);
void lodepng_free(void* ptr);
#endif /*LODEPNG_COMPILE_ALLOCATORS*/

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* // Tools for C, and common code for PNG and Zlib.                       // */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */

/*
Often in case of an error a value is assigned to a variable and then it breaks
out of a loop (to go to the cleanup phase of a function). This macro does that.
It makes the error handling code shorter and more readable.

Example: if(!uivector_resizev(&frequencies_ll, 286, 0)) ERROR_BREAK(83);
*/
#define CERROR_BREAK(errorvar, code)\
{\
  errorvar = code;\
  break;\
}

/*version of CERROR_BREAK that assumes the common case where the error variable is named "error"*/
#define ERROR_BREAK(code) CERROR_BREAK(error, code)

/*Set error var to the error code, and return it.*/
#define CERROR_RETURN_ERROR(errorvar, code)\
{\
  errorvar = code;\
  return code;\
}

/*Try the code, if it returns error, also return the error.*/
#define CERROR_TRY_RETURN(call)\
{\
  unsigned error = call;\
  if(error) return error;\
}

/*
About uivector, ucvector and string:
-All of them wrap dynamic arrays or text strings in a similar way.
-LodePNG was originally written in C++. The vectors replace the std::vectors that were used in the C++ version.
-The string tools are made to avoid problems with compilers that declare things like strncat as deprecated.
-They're not used in the interface, only internally in this file as static functions.
-As with many other structs in this file, the init and cleanup functions serve as ctor and dtor.
*/

#ifdef LODEPNG_COMPILE_ZLIB
/*dynamic vector of unsigned ints*/
typedef struct uivector
{
  unsigned* data;
  size_t size; /*size in number of unsigned longs*/
  size_t allocsize; /*allocated size in bytes*/
} uivector;

static void uivector_cleanup(void* p)
{
  ((uivector*)p)->size = ((uivector*)p)->allocsize = 0;
  lodepng_free(((uivector*)p)->data);
  ((uivector*)p)->data = NULL;
}

/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_resize(uivector* p, size_t size)
{
  if(size * sizeof(unsigned) > p->allocsize)
  {
    size_t newsize = size * sizeof(unsigned) * 2;
    void* data = lodepng_realloc(p->data, newsize);
    if(data)
    {
      p->allocsize = newsize;
      p->data = (unsigned*)data;
      p->size = size;
    }
    else return 0;
  }
  else p->size = size;
  return 1;
}

/*resize and give all new elements the value*/
static unsigned uivector_resizev(uivector* p, size_t size, unsigned value)
{
  size_t oldsize = p->size, i;
  if(!uivector_resize(p, size)) return 0;
  for(i = oldsize; i < size; i++) p->data[i] = value;
  return 1;
}

static void uivector_init(uivector* p)
{
  p->data = NULL;
  p->size = p->allocsize = 0;
}

#ifdef LODEPNG_COMPILE_ENCODER
/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_push_back(uivector* p, unsigned c)
{
  if(!uivector_resize(p, p->size + 1)) return 0;
  p->data[p->size - 1] = c;
  return 1;
}

/*copy q to p, returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_copy(uivector* p, const uivector* q)
{
  size_t i;
  if(!uivector_resize(p, q->size)) return 0;
  for(i = 0; i < q->size; i++) p->data[i] = q->data[i];
  return 1;
}

static void uivector_swap(uivector* p, uivector* q)
{
  size_t tmp;
  unsigned* tmpp;
  tmp = p->size; p->size = q->size; q->size = tmp;
  tmp = p->allocsize; p->allocsize = q->allocsize; q->allocsize = tmp;
  tmpp = p->data; p->data = q->data; q->data = tmpp;
}
#endif /*LODEPNG_COMPILE_ENCODER*/
#endif /*LODEPNG_COMPILE_ZLIB*/

/* /////////////////////////////////////////////////////////////////////////// */

/*dynamic vector of unsigned chars*/
typedef struct ucvector
{
  unsigned char* data;
  size_t size; /*used size*/
  size_t allocsize; /*allocated size*/
} ucvector;

/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned ucvector_resize(ucvector* p, size_t size)
{
  if(size * sizeof(unsigned char) > p->allocsize)
  {
    size_t newsize = size * sizeof(unsigned char) * 2;
    void* data = lodepng_realloc(p->data, newsize);
    if(data)
    {
      p->allocsize = newsize;
      p->data = (unsigned char*)data;
      p->size = size;
    }
    else return 0; /*error: not enough memory*/
  }
  else p->size = size;
  return 1;
}

#ifdef LODEPNG_COMPILE_PNG

static void ucvector_cleanup(void* p)
{
  ((ucvector*)p)->size = ((ucvector*)p)->allocsize = 0;
  lodepng_free(((ucvector*)p)->data);
  ((ucvector*)p)->data = NULL;
}

static void ucvector_init(ucvector* p)
{
  p->data = NULL;
  p->size = p->allocsize = 0;
}

#ifdef LODEPNG_COMPILE_DECODER
/*resize and give all new elements the value*/
static unsigned ucvector_resizev(ucvector* p, size_t size, unsigned char value)
{
  size_t oldsize = p->size, i;
  if(!ucvector_resize(p, size)) return 0;
  for(i = oldsize; i < size; i++) p->data[i] = value;
  return 1;
}
#endif /*LODEPNG_COMPILE_DECODER*/
#endif /*LODEPNG_COMPILE_PNG*/

#ifdef LODEPNG_COMPILE_ZLIB
/*you can both convert from vector to buffer&size and vica versa. If you use
init_buffer to take over a buffer and size, it is not needed to use cleanup*/
static void ucvector_init_buffer(ucvector* p, unsigned char* buffer, size_t size)
{
  p->data = buffer;
  p->allocsize = p->size = size;
}
#endif /*LODEPNG_COMPILE_ZLIB*/

#if (defined(LODEPNG_COMPILE_PNG) && defined(LODEPNG_COMPILE_ANCILLARY_CHUNKS)) || defined(LODEPNG_COMPILE_ENCODER)
/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned ucvector_push_back(ucvector* p, unsigned char c)
{
  if(!ucvector_resize(p, p->size + 1)) return 0;
  p->data[p->size - 1] = c;
  return 1;
}
#endif /*defined(LODEPNG_COMPILE_PNG) || defined(LODEPNG_COMPILE_ENCODER)*/


/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_PNG
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned string_resize(char** out, size_t size)
{
  char* data = (char*)lodepng_realloc(*out, size + 1);
  if(data)
  {
    data[size] = 0; /*null termination char*/
    *out = data;
  }
  return data != 0;
}

/*init a {char*, size_t} pair for use as string*/
static void string_init(char** out)
{
  *out = NULL;
  string_resize(out, 0);
}

/*free the above pair again*/
static void string_cleanup(char** out)
{
  lodepng_free(*out);
  *out = NULL;
}

static void string_set(char** out, const char* in)
{
  size_t insize = strlen(in), i = 0;
  if(string_resize(out, insize))
  {
    for(i = 0; i < insize; i++)
    {
      (*out)[i] = in[i];
    }
  }
}
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
#endif /*LODEPNG_COMPILE_PNG*/

/* ////////////////////////////////////////////////////////////////////////// */

unsigned lodepng_read32bitInt(const unsigned char* buffer)
{
  return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
}

#if defined(LODEPNG_COMPILE_PNG) || defined(LODEPNG_COMPILE_ENCODER)
/*buffer must have at least 4 allocated bytes available*/
static void lodepng_set32bitInt(unsigned char* buffer, unsigned value)
{
  buffer[0] = (unsigned char)((value >> 24) & 0xff);
  buffer[1] = (unsigned char)((value >> 16) & 0xff);
  buffer[2] = (unsigned char)((value >>  8) & 0xff);
  buffer[3] = (unsigned char)((value      ) & 0xff);
}
#endif /*defined(LODEPNG_COMPILE_PNG) || defined(LODEPNG_COMPILE_ENCODER)*/

#ifdef LODEPNG_COMPILE_ENCODER
static void lodepng_add32bitInt(ucvector* buffer, unsigned value)
{
  ucvector_resize(buffer, buffer->size + 4); /*todo: give error if resize failed*/
  lodepng_set32bitInt(&buffer->data[buffer->size - 4], value);
}
#endif /*LODEPNG_COMPILE_ENCODER*/

/* ////////////////////////////////////////////////////////////////////////// */
/* / File IO                                                                / */
/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_DISK

unsigned lodepng_load_file(unsigned char** out, size_t* outsize, const char* filename)
{
  FILE* file;
  long size;

  /*provide some proper output values if error will happen*/
  *out = 0;
  *outsize = 0;

  file = fopen(filename, "rb");
  if(!file) return 78;

  /*get filesize:*/
  fseek(file , 0 , SEEK_END);
  size = ftell(file);
  rewind(file);

  /*read contents of the file into the vector*/
  *outsize = 0;
  *out = (unsigned char*)lodepng_malloc((size_t)size);
  if(size && (*out)) (*outsize) = fread(*out, 1, (size_t)size, file);

  fclose(file);
  if(!(*out) && size) return 83; /*the above malloc failed*/
  return 0;
}

/*write given buffer to the file, overwriting the file, it doesn't append to it.*/
unsigned lodepng_save_file(const unsigned char* buffer, size_t buffersize, const char* filename)
{
  FILE* file;
  file = fopen(filename, "wb" );
  if(!file) return 79;
  fwrite((char*)buffer , 1 , buffersize, file);
  fclose(file);
  return 0;
}

#endif /*LODEPNG_COMPILE_DISK*/

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* // End of common code and tools. Begin of Zlib related code.            // */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_ZLIB
#ifdef LODEPNG_COMPILE_ENCODER
/*TODO: this ignores potential out of memory errors*/
static void addBitToStream(size_t* bitpointer, ucvector* bitstream, unsigned char bit)
{
  /*add a new byte at the end*/
  if((*bitpointer) % 8 == 0) ucvector_push_back(bitstream, (unsigned char)0);
  /*earlier bit of huffman code is in a lesser significant bit of an earlier byte*/
  (bitstream->data[bitstream->size - 1]) |= (bit << ((*bitpointer) & 0x7));
  (*bitpointer)++;
}

static void addBitsToStream(size_t* bitpointer, ucvector* bitstream, unsigned value, size_t nbits)
{
  size_t i;
  for(i = 0; i < nbits; i++) addBitToStream(bitpointer, bitstream, (unsigned char)((value >> i) & 1));
}

static void addBitsToStreamReversed(size_t* bitpointer, ucvector* bitstream, unsigned value, size_t nbits)
{
  size_t i;
  for(i = 0; i < nbits; i++) addBitToStream(bitpointer, bitstream, (unsigned char)((value >> (nbits - 1 - i)) & 1));
}
#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_DECODER

#define READBIT(bitpointer, bitstream) ((bitstream[bitpointer >> 3] >> (bitpointer & 0x7)) & (unsigned char)1)

static unsigned char readBitFromStream(size_t* bitpointer, const unsigned char* bitstream)
{
  unsigned char result = (unsigned char)(READBIT(*bitpointer, bitstream));
  (*bitpointer)++;
  return result;
}

static unsigned readBitsFromStream(size_t* bitpointer, const unsigned char* bitstream, size_t nbits)
{
  unsigned result = 0, i;
  for(i = 0; i < nbits; i++)
  {
    result += ((unsigned)READBIT(*bitpointer, bitstream)) << i;
    (*bitpointer)++;
  }
  return result;
}
#endif /*LODEPNG_COMPILE_DECODER*/

/* ////////////////////////////////////////////////////////////////////////// */
/* / Deflate - Huffman                                                      / */
/* ////////////////////////////////////////////////////////////////////////// */

#define FIRST_LENGTH_CODE_INDEX 257
#define LAST_LENGTH_CODE_INDEX 285
/*256 literals, the end code, some length codes, and 2 unused codes*/
#define NUM_DEFLATE_CODE_SYMBOLS 288
/*the distance codes have their own symbols, 30 used, 2 unused*/
#define NUM_DISTANCE_SYMBOLS 32
/*the code length codes. 0-15: code lengths, 16: copy previous 3-6 times, 17: 3-10 zeros, 18: 11-138 zeros*/
#define NUM_CODE_LENGTH_CODES 19

/*the base lengths represented by codes 257-285*/
static const unsigned LENGTHBASE[29]
  = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
     67, 83, 99, 115, 131, 163, 195, 227, 258};

/*the extra bits used by codes 257-285 (added to base length)*/
static const unsigned LENGTHEXTRA[29]
  = {0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,
      4,  4,  4,   4,   5,   5,   5,   5,   0};

/*the base backwards distances (the bits of distance codes appear after length codes and use their own huffman tree)*/
static const unsigned DISTANCEBASE[30]
  = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513,
     769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};

/*the extra bits of backwards distances (added to base)*/
static const unsigned DISTANCEEXTRA[30]
  = {0, 0, 0, 0, 1, 1, 2,  2,  3,  3,  4,  4,  5,  5,   6,   6,   7,   7,   8,
       8,    9,    9,   10,   10,   11,   11,   12,    12,    13,    13};

/*the order in which "code length alphabet code lengths" are stored, out of this
the huffman tree of the dynamic huffman tree lengths is generated*/
static const unsigned CLCL_ORDER[NUM_CODE_LENGTH_CODES]
  = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

/* ////////////////////////////////////////////////////////////////////////// */

/*
Huffman tree struct, containing multiple representations of the tree
*/
typedef struct HuffmanTree
{
  unsigned* tree2d;
  unsigned* tree1d;
  unsigned* lengths; /*the lengths of the codes of the 1d-tree*/
  unsigned maxbitlen; /*maximum number of bits a single code can get*/
  unsigned numcodes; /*number of symbols in the alphabet = number of codes*/
} HuffmanTree;

/*function used for debug purposes to draw the tree in ascii art with C++*/
/*
static void HuffmanTree_draw(HuffmanTree* tree)
{
  std::cout << "tree. length: " << tree->numcodes << " maxbitlen: " << tree->maxbitlen << std::endl;
  for(size_t i = 0; i < tree->tree1d.size; i++)
  {
    if(tree->lengths.data[i])
      std::cout << i << " " << tree->tree1d.data[i] << " " << tree->lengths.data[i] << std::endl;
  }
  std::cout << std::endl;
}*/

static void HuffmanTree_init(HuffmanTree* tree)
{
  tree->tree2d = 0;
  tree->tree1d = 0;
  tree->lengths = 0;
}

static void HuffmanTree_cleanup(HuffmanTree* tree)
{
  lodepng_free(tree->tree2d);
  lodepng_free(tree->tree1d);
  lodepng_free(tree->lengths);
}

/*the tree representation used by the decoder. return value is error*/
static unsigned HuffmanTree_make2DTree(HuffmanTree* tree)
{
  unsigned nodefilled = 0; /*up to which node it is filled*/
  unsigned treepos = 0; /*position in the tree (1 of the numcodes columns)*/
  unsigned n, i;

  tree->tree2d = (unsigned*)lodepng_malloc(tree->numcodes * 2 * sizeof(unsigned));
  if(!tree->tree2d) return 83; /*alloc fail*/

  /*
  convert tree1d[] to tree2d[][]. In the 2D array, a value of 32767 means
  uninited, a value >= numcodes is an address to another bit, a value < numcodes
  is a code. The 2 rows are the 2 possible bit values (0 or 1), there are as
  many columns as codes - 1.
  A good huffmann tree has N * 2 - 1 nodes, of which N - 1 are internal nodes.
  Here, the internal nodes are stored (what their 0 and 1 option point to).
  There is only memory for such good tree currently, if there are more nodes
  (due to too long length codes), error 55 will happen
  */
  for(n = 0; n < tree->numcodes * 2; n++)
  {
    tree->tree2d[n] = 32767; /*32767 here means the tree2d isn't filled there yet*/
  }

  for(n = 0; n < tree->numcodes; n++) /*the codes*/
  {
    for(i = 0; i < tree->lengths[n]; i++) /*the bits for this code*/
    {
      unsigned char bit = (unsigned char)((tree->tree1d[n] >> (tree->lengths[n] - i - 1)) & 1);
      if(treepos > tree->numcodes - 2) return 55; /*oversubscribed, see comment in lodepng_error_text*/
      if(tree->tree2d[2 * treepos + bit] == 32767) /*not yet filled in*/
      {
        if(i + 1 == tree->lengths[n]) /*last bit*/
        {
          tree->tree2d[2 * treepos + bit] = n; /*put the current code in it*/
          treepos = 0;
        }
        else
        {
          /*put address of the next step in here, first that address has to be found of course
          (it's just nodefilled + 1)...*/
          nodefilled++;
          /*addresses encoded with numcodes added to it*/
          tree->tree2d[2 * treepos + bit] = nodefilled + tree->numcodes;
          treepos = nodefilled;
        }
      }
      else treepos = tree->tree2d[2 * treepos + bit] - tree->numcodes;
    }
  }

  for(n = 0;  n < tree->numcodes * 2; n++)
  {
    if(tree->tree2d[n] == 32767) tree->tree2d[n] = 0; /*remove possible remaining 32767's*/
  }

  return 0;
}

/*
Second step for the ...makeFromLengths and ...makeFromFrequencies functions.
numcodes, lengths and maxbitlen must already be filled in correctly. return
value is error.
*/
static unsigned HuffmanTree_makeFromLengths2(HuffmanTree* tree)
{
  uivector blcount;
  uivector nextcode;
  unsigned bits, n, error = 0;

  uivector_init(&blcount);
  uivector_init(&nextcode);

  tree->tree1d = (unsigned*)lodepng_malloc(tree->numcodes * sizeof(unsigned));
  if(!tree->tree1d) error = 83; /*alloc fail*/

  if(!uivector_resizev(&blcount, tree->maxbitlen + 1, 0)
  || !uivector_resizev(&nextcode, tree->maxbitlen + 1, 0))
    error = 83; /*alloc fail*/

  if(!error)
  {
    /*step 1: count number of instances of each code length*/
    for(bits = 0; bits < tree->numcodes; bits++) blcount.data[tree->lengths[bits]]++;
    /*step 2: generate the nextcode values*/
    for(bits = 1; bits <= tree->maxbitlen; bits++)
    {
      nextcode.data[bits] = (nextcode.data[bits - 1] + blcount.data[bits - 1]) << 1;
    }
    /*step 3: generate all the codes*/
    for(n = 0; n < tree->numcodes; n++)
    {
      if(tree->lengths[n] != 0) tree->tree1d[n] = nextcode.data[tree->lengths[n]]++;
    }
  }

  uivector_cleanup(&blcount);
  uivector_cleanup(&nextcode);

  if(!error) return HuffmanTree_make2DTree(tree);
  else return error;
}

/*
given the code lengths (as stored in the PNG file), generate the tree as defined
by Deflate. maxbitlen is the maximum bits that a code in the tree can have.
return value is error.
*/
static unsigned HuffmanTree_makeFromLengths(HuffmanTree* tree, const unsigned* bitlen,
                                            size_t numcodes, unsigned maxbitlen)
{
  unsigned i;
  tree->lengths = (unsigned*)lodepng_malloc(numcodes * sizeof(unsigned));
  if(!tree->lengths) return 83; /*alloc fail*/
  for(i = 0; i < numcodes; i++) tree->lengths[i] = bitlen[i];
  tree->numcodes = (unsigned)numcodes; /*number of symbols*/
  tree->maxbitlen = maxbitlen;
  return HuffmanTree_makeFromLengths2(tree);
}

#ifdef LODEPNG_COMPILE_ENCODER

/*
A coin, this is the terminology used for the package-merge algorithm and the
coin collector's problem. This is used to generate the huffman tree.
A coin can be multiple coins (when they're merged)
*/
typedef struct Coin
{
  uivector symbols;
  float weight; /*the sum of all weights in this coin*/
} Coin;

static void coin_init(Coin* c)
{
  uivector_init(&c->symbols);
}

/*argument c is void* so that this dtor can be given as function pointer to the vector resize function*/
static void coin_cleanup(void* c)
{
  uivector_cleanup(&((Coin*)c)->symbols);
}

static void coin_copy(Coin* c1, const Coin* c2)
{
  c1->weight = c2->weight;
  uivector_copy(&c1->symbols, &c2->symbols);
}

static void add_coins(Coin* c1, const Coin* c2)
{
  size_t i;
  for(i = 0; i < c2->symbols.size; i++) uivector_push_back(&c1->symbols, c2->symbols.data[i]);
  c1->weight += c2->weight;
}

static void init_coins(Coin* coins, size_t num)
{
  size_t i;
  for(i = 0; i < num; i++) coin_init(&coins[i]);
}

static void cleanup_coins(Coin* coins, size_t num)
{
  size_t i;
  for(i = 0; i < num; i++) coin_cleanup(&coins[i]);
}

/*
This uses a simple combsort to sort the data. This function is not critical for
overall encoding speed and the data amount isn't that large.
*/
static void sort_coins(Coin* data, size_t amount)
{
  size_t gap = amount;
  unsigned char swapped = 0;
  while((gap > 1) || swapped)
  {
    size_t i;
    gap = (gap * 10) / 13; /*shrink factor 1.3*/
    if(gap == 9 || gap == 10) gap = 11; /*combsort11*/
    if(gap < 1) gap = 1;
    swapped = 0;
    for(i = 0; i < amount - gap; i++)
    {
      size_t j = i + gap;
      if(data[j].weight < data[i].weight)
      {
        float temp = data[j].weight; data[j].weight = data[i].weight; data[i].weight = temp;
        uivector_swap(&data[i].symbols, &data[j].symbols);
        swapped = 1;
      }
    }
  }
}

static unsigned append_symbol_coins(Coin* coins, const unsigned* frequencies, unsigned numcodes, size_t sum)
{
  unsigned i;
  unsigned j = 0; /*index of present symbols*/
  for(i = 0; i < numcodes; i++)
  {
    if(frequencies[i] != 0) /*only include symbols that are present*/
    {
      coins[j].weight = frequencies[i] / (float)sum;
      uivector_push_back(&coins[j].symbols, i);
      j++;
    }
  }
  return 0;
}

unsigned lodepng_huffman_code_lengths(unsigned* lengths, const unsigned* frequencies,
                                      size_t numcodes, unsigned maxbitlen)
{
  unsigned i, j;
  size_t sum = 0, numpresent = 0;
  unsigned error = 0;
  Coin* coins; /*the coins of the currently calculated row*/
  Coin* prev_row; /*the previous row of coins*/
  unsigned numcoins;
  unsigned coinmem;

  if(numcodes == 0) return 80; /*error: a tree of 0 symbols is not supposed to be made*/

  for(i = 0; i < numcodes; i++)
  {
    if(frequencies[i] > 0)
    {
      numpresent++;
      sum += frequencies[i];
    }
  }

  for(i = 0; i < numcodes; i++) lengths[i] = 0;

  /*ensure at least two present symbols. There should be at least one symbol
  according to RFC 1951 section 3.2.7. To decoders incorrectly require two. To
  make these work as well ensure there are at least two symbols. The
  Package-Merge code below also doesn't work correctly if there's only one
  symbol, it'd give it the theoritical 0 bits but in practice zlib wants 1 bit*/
  if(numpresent == 0)
  {
    lengths[0] = lengths[1] = 1; /*note that for RFC 1951 section 3.2.7, only lengths[0] = 1 is needed*/
  }
  else if(numpresent == 1)
  {
    for(i = 0; i < numcodes; i++)
    {
      if(frequencies[i])
      {
        lengths[i] = 1;
        lengths[i == 0 ? 1 : 0] = 1;
        break;
      }
    }
  }
  else
  {
    /*Package-Merge algorithm represented by coin collector's problem
    For every symbol, maxbitlen coins will be created*/

    coinmem = numpresent * 2; /*max amount of coins needed with the current algo*/
    coins = (Coin*)lodepng_malloc(sizeof(Coin) * coinmem);
    prev_row = (Coin*)lodepng_malloc(sizeof(Coin) * coinmem);
    if(!coins || !prev_row)
    {
      lodepng_free(coins);
      lodepng_free(prev_row);
      return 83; /*alloc fail*/
    }
    init_coins(coins, coinmem);
    init_coins(prev_row, coinmem);

    /*first row, lowest denominator*/
    error = append_symbol_coins(coins, frequencies, numcodes, sum);
    numcoins = numpresent;
    sort_coins(coins, numcoins);
    if(!error)
    {
      unsigned numprev = 0;
      for(j = 1; j <= maxbitlen && !error; j++) /*each of the remaining rows*/
      {
        unsigned tempnum;
        Coin* tempcoins;
        /*swap prev_row and coins, and their amounts*/
        tempcoins = prev_row; prev_row = coins; coins = tempcoins;
        tempnum = numprev; numprev = numcoins; numcoins = tempnum;

        cleanup_coins(coins, numcoins);
        init_coins(coins, numcoins);

        numcoins = 0;

        /*fill in the merged coins of the previous row*/
        for(i = 0; i + 1 < numprev; i += 2)
        {
          /*merge prev_row[i] and prev_row[i + 1] into new coin*/
          Coin* coin = &coins[numcoins++];
          coin_copy(coin, &prev_row[i]);
          add_coins(coin, &prev_row[i + 1]);
        }
        /*fill in all the original symbols again*/
        if(j < maxbitlen)
        {
          error = append_symbol_coins(coins + numcoins, frequencies, numcodes, sum);
          numcoins += numpresent;
        }
        sort_coins(coins, numcoins);
      }
    }

    if(!error)
    {
      /*calculate the lenghts of each symbol, as the amount of times a coin of each symbol is used*/
      for(i = 0; i < numpresent - 1; i++)
      {
        Coin* coin = &coins[i];
        for(j = 0; j < coin->symbols.size; j++) lengths[coin->symbols.data[j]]++;
      }
    }

    cleanup_coins(coins, coinmem);
    lodepng_free(coins);
    cleanup_coins(prev_row, coinmem);
    lodepng_free(prev_row);
  }

  return error;
}

/*Create the Huffman tree given the symbol frequencies*/
static unsigned HuffmanTree_makeFromFrequencies(HuffmanTree* tree, const unsigned* frequencies,
                                                size_t mincodes, size_t numcodes, unsigned maxbitlen)
{
  unsigned error = 0;
  while(!frequencies[numcodes - 1] && numcodes > mincodes) numcodes--; /*trim zeroes*/
  tree->maxbitlen = maxbitlen;
  tree->numcodes = (unsigned)numcodes; /*number of symbols*/
  tree->lengths = (unsigned*)lodepng_realloc(tree->lengths, numcodes * sizeof(unsigned));
  if(!tree->lengths) return 83; /*alloc fail*/
  /*initialize all lengths to 0*/
  memset(tree->lengths, 0, numcodes * sizeof(unsigned));

  error = lodepng_huffman_code_lengths(tree->lengths, frequencies, numcodes, maxbitlen);
  if(!error) error = HuffmanTree_makeFromLengths2(tree);
  return error;
}

static unsigned HuffmanTree_getCode(const HuffmanTree* tree, unsigned index)
{
  return tree->tree1d[index];
}

static unsigned HuffmanTree_getLength(const HuffmanTree* tree, unsigned index)
{
  return tree->lengths[index];
}
#endif /*LODEPNG_COMPILE_ENCODER*/

/*get the literal and length code tree of a deflated block with fixed tree, as per the deflate specification*/
static unsigned generateFixedLitLenTree(HuffmanTree* tree)
{
  unsigned i, error = 0;
  unsigned* bitlen = (unsigned*)lodepng_malloc(NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));
  if(!bitlen) return 83; /*alloc fail*/

  /*288 possible codes: 0-255=literals, 256=endcode, 257-285=lengthcodes, 286-287=unused*/
  for(i =   0; i <= 143; i++) bitlen[i] = 8;
  for(i = 144; i <= 255; i++) bitlen[i] = 9;
  for(i = 256; i <= 279; i++) bitlen[i] = 7;
  for(i = 280; i <= 287; i++) bitlen[i] = 8;

  error = HuffmanTree_makeFromLengths(tree, bitlen, NUM_DEFLATE_CODE_SYMBOLS, 15);

  lodepng_free(bitlen);
  return error;
}

/*get the distance code tree of a deflated block with fixed tree, as specified in the deflate specification*/
static unsigned generateFixedDistanceTree(HuffmanTree* tree)
{
  unsigned i, error = 0;
  unsigned* bitlen = (unsigned*)lodepng_malloc(NUM_DISTANCE_SYMBOLS * sizeof(unsigned));
  if(!bitlen) return 83; /*alloc fail*/

  /*there are 32 distance codes, but 30-31 are unused*/
  for(i = 0; i < NUM_DISTANCE_SYMBOLS; i++) bitlen[i] = 5;
  error = HuffmanTree_makeFromLengths(tree, bitlen, NUM_DISTANCE_SYMBOLS, 15);

  lodepng_free(bitlen);
  return error;
}

#ifdef LODEPNG_COMPILE_DECODER

/*
returns the code, or (unsigned)(-1) if error happened
inbitlength is the length of the complete buffer, in bits (so its byte length times 8)
*/
static unsigned huffmanDecodeSymbol(const unsigned char* in, size_t* bp,
                                    const HuffmanTree* codetree, size_t inbitlength)
{
  unsigned treepos = 0, ct;
  for(;;)
  {
    if(*bp >= inbitlength) return (unsigned)(-1); /*error: end of input memory reached without endcode*/
    /*
    decode the symbol from the tree. The "readBitFromStream" code is inlined in
    the expression below because this is the biggest bottleneck while decoding
    */
    ct = codetree->tree2d[(treepos << 1) + READBIT(*bp, in)];
    (*bp)++;
    if(ct < codetree->numcodes) return ct; /*the symbol is decoded, return it*/
    else treepos = ct - codetree->numcodes; /*symbol not yet decoded, instead move tree position*/

    if(treepos >= codetree->numcodes) return (unsigned)(-1); /*error: it appeared outside the codetree*/
  }
}
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_DECODER

/* ////////////////////////////////////////////////////////////////////////// */
/* / Inflator (Decompressor)                                                / */
/* ////////////////////////////////////////////////////////////////////////// */

/*get the tree of a deflated block with fixed tree, as specified in the deflate specification*/
static void getTreeInflateFixed(HuffmanTree* tree_ll, HuffmanTree* tree_d)
{
  /*TODO: check for out of memory errors*/
  generateFixedLitLenTree(tree_ll);
  generateFixedDistanceTree(tree_d);
}

/*get the tree of a deflated block with dynamic tree, the tree itself is also Huffman compressed with a known tree*/
static unsigned getTreeInflateDynamic(HuffmanTree* tree_ll, HuffmanTree* tree_d,
                                      const unsigned char* in, size_t* bp, size_t inlength)
{
  /*make sure that length values that aren't filled in will be 0, or a wrong tree will be generated*/
  unsigned error = 0;
  unsigned n, HLIT, HDIST, HCLEN, i;
  size_t inbitlength = inlength * 8;

  /*see comments in deflateDynamic for explanation of the context and these variables, it is analogous*/
  unsigned* bitlen_ll = 0; /*lit,len code lengths*/
  unsigned* bitlen_d = 0; /*dist code lengths*/
  /*code length code lengths ("clcl"), the bit lengths of the huffman tree used to compress bitlen_ll and bitlen_d*/
  unsigned* bitlen_cl = 0;
  HuffmanTree tree_cl; /*the code tree for code length codes (the huffman tree for compressed huffman trees)*/

  if((*bp) >> 3 >= inlength - 2) return 49; /*error: the bit pointer is or will go past the memory*/

  /*number of literal/length codes + 257. Unlike the spec, the value 257 is added to it here already*/
  HLIT =  readBitsFromStream(bp, in, 5) + 257;
  /*number of distance codes. Unlike the spec, the value 1 is added to it here already*/
  HDIST = readBitsFromStream(bp, in, 5) + 1;
  /*number of code length codes. Unlike the spec, the value 4 is added to it here already*/
  HCLEN = readBitsFromStream(bp, in, 4) + 4;

  HuffmanTree_init(&tree_cl);

  while(!error)
  {
    /*read the code length codes out of 3 * (amount of code length codes) bits*/

    bitlen_cl = (unsigned*)lodepng_malloc(NUM_CODE_LENGTH_CODES * sizeof(unsigned));
    if(!bitlen_cl) ERROR_BREAK(83 /*alloc fail*/);

    for(i = 0; i < NUM_CODE_LENGTH_CODES; i++)
    {
      if(i < HCLEN) bitlen_cl[CLCL_ORDER[i]] = readBitsFromStream(bp, in, 3);
      else bitlen_cl[CLCL_ORDER[i]] = 0; /*if not, it must stay 0*/
    }

    error = HuffmanTree_makeFromLengths(&tree_cl, bitlen_cl, NUM_CODE_LENGTH_CODES, 7);
    if(error) break;

    /*now we can use this tree to read the lengths for the tree that this function will return*/
    bitlen_ll = (unsigned*)lodepng_malloc(NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));
    bitlen_d = (unsigned*)lodepng_malloc(NUM_DISTANCE_SYMBOLS * sizeof(unsigned));
    if(!bitlen_ll || !bitlen_d) ERROR_BREAK(83 /*alloc fail*/);
    for(i = 0; i < NUM_DEFLATE_CODE_SYMBOLS; i++) bitlen_ll[i] = 0;
    for(i = 0; i < NUM_DISTANCE_SYMBOLS; i++) bitlen_d[i] = 0;

    /*i is the current symbol we're reading in the part that contains the code lengths of lit/len and dist codes*/
    i = 0;
    while(i < HLIT + HDIST)
    {
      unsigned code = huffmanDecodeSymbol(in, bp, &tree_cl, inbitlength);
      if(code <= 15) /*a length code*/
      {
        if(i < HLIT) bitlen_ll[i] = code;
        else bitlen_d[i - HLIT] = code;
        i++;
      }
      else if(code == 16) /*repeat previous*/
      {
        unsigned replength = 3; /*read in the 2 bits that indicate repeat length (3-6)*/
        unsigned value; /*set value to the previous code*/

        if(*bp >= inbitlength) ERROR_BREAK(50); /*error, bit pointer jumps past memory*/
        if (i == 0) ERROR_BREAK(54); /*can't repeat previous if i is 0*/

        replength += readBitsFromStream(bp, in, 2);

        if(i < HLIT + 1) value = bitlen_ll[i - 1];
        else value = bitlen_d[i - HLIT - 1];
        /*repeat this value in the next lengths*/
        for(n = 0; n < replength; n++)
        {
          if(i >= HLIT + HDIST) ERROR_BREAK(13); /*error: i is larger than the amount of codes*/
          if(i < HLIT) bitlen_ll[i] = value;
          else bitlen_d[i - HLIT] = value;
          i++;
        }
      }
      else if(code == 17) /*repeat "0" 3-10 times*/
      {
        unsigned replength = 3; /*read in the bits that indicate repeat length*/
        if(*bp >= inbitlength) ERROR_BREAK(50); /*error, bit pointer jumps past memory*/

        replength += readBitsFromStream(bp, in, 3);

        /*repeat this value in the next lengths*/
        for(n = 0; n < replength; n++)
        {
          if(i >= HLIT + HDIST) ERROR_BREAK(14); /*error: i is larger than the amount of codes*/

          if(i < HLIT) bitlen_ll[i] = 0;
          else bitlen_d[i - HLIT] = 0;
          i++;
        }
      }
      else if(code == 18) /*repeat "0" 11-138 times*/
      {
        unsigned replength = 11; /*read in the bits that indicate repeat length*/
        if(*bp >= inbitlength) ERROR_BREAK(50); /*error, bit pointer jumps past memory*/

        replength += readBitsFromStream(bp, in, 7);

        /*repeat this value in the next lengths*/
        for(n = 0; n < replength; n++)
        {
          if(i >= HLIT + HDIST) ERROR_BREAK(15); /*error: i is larger than the amount of codes*/

          if(i < HLIT) bitlen_ll[i] = 0;
          else bitlen_d[i - HLIT] = 0;
          i++;
        }
      }
      else /*if(code == (unsigned)(-1))*/ /*huffmanDecodeSymbol returns (unsigned)(-1) in case of error*/
      {
        if(code == (unsigned)(-1))
        {
          /*return error code 10 or 11 depending on the situation that happened in huffmanDecodeSymbol
          (10=no endcode, 11=wrong jump outside of tree)*/
          error = (*bp) > inbitlength ? 10 : 11;
        }
        else error = 16; /*unexisting code, this can never happen*/
        break;
      }
    }
    if(error) break;

    if(bitlen_ll[256] == 0) ERROR_BREAK(64); /*the length of the end code 256 must be larger than 0*/

    /*now we've finally got HLIT and HDIST, so generate the code trees, and the function is done*/
    error = HuffmanTree_makeFromLengths(tree_ll, bitlen_ll, NUM_DEFLATE_CODE_SYMBOLS, 15);
    if(error) break;
    error = HuffmanTree_makeFromLengths(tree_d, bitlen_d, NUM_DISTANCE_SYMBOLS, 15);

    break; /*end of error-while*/
  }

  lodepng_free(bitlen_cl);
  lodepng_free(bitlen_ll);
  lodepng_free(bitlen_d);
  HuffmanTree_cleanup(&tree_cl);

  return error;
}

/*inflate a block with dynamic of fixed Huffman tree*/
static unsigned inflateHuffmanBlock(ucvector* out, const unsigned char* in, size_t* bp,
                                    size_t* pos, size_t inlength, unsigned btype)
{
  unsigned error = 0;
  HuffmanTree tree_ll; /*the huffman tree for literal and length codes*/
  HuffmanTree tree_d; /*the huffman tree for distance codes*/
  size_t inbitlength = inlength * 8;

  HuffmanTree_init(&tree_ll);
  HuffmanTree_init(&tree_d);

  if(btype == 1) getTreeInflateFixed(&tree_ll, &tree_d);
  else if(btype == 2) error = getTreeInflateDynamic(&tree_ll, &tree_d, in, bp, inlength);

  while(!error) /*decode all symbols until end reached, breaks at end code*/
  {
    /*code_ll is literal, length or end code*/
    unsigned code_ll = huffmanDecodeSymbol(in, bp, &tree_ll, inbitlength);
    if(code_ll <= 255) /*literal symbol*/
    {
      if((*pos) >= out->size)
      {
        /*reserve more room at once*/
        if(!ucvector_resize(out, ((*pos) + 1) * 2)) ERROR_BREAK(83 /*alloc fail*/);
      }
      out->data[(*pos)] = (unsigned char)(code_ll);
      (*pos)++;
    }
    else if(code_ll >= FIRST_LENGTH_CODE_INDEX && code_ll <= LAST_LENGTH_CODE_INDEX) /*length code*/
    {
      unsigned code_d, distance;
      unsigned numextrabits_l, numextrabits_d; /*extra bits for length and distance*/
      size_t start, forward, backward, length;

      /*part 1: get length base*/
      length = LENGTHBASE[code_ll - FIRST_LENGTH_CODE_INDEX];

      /*part 2: get extra bits and add the value of that to length*/
      numextrabits_l = LENGTHEXTRA[code_ll - FIRST_LENGTH_CODE_INDEX];
      if(*bp >= inbitlength) ERROR_BREAK(51); /*error, bit pointer will jump past memory*/
      length += readBitsFromStream(bp, in, numextrabits_l);

      /*part 3: get distance code*/
      code_d = huffmanDecodeSymbol(in, bp, &tree_d, inbitlength);
      if(code_d > 29)
      {
        if(code_ll == (unsigned)(-1)) /*huffmanDecodeSymbol returns (unsigned)(-1) in case of error*/
        {
          /*return error code 10 or 11 depending on the situation that happened in huffmanDecodeSymbol
          (10=no endcode, 11=wrong jump outside of tree)*/
          error = (*bp) > inlength * 8 ? 10 : 11;
        }
        else error = 18; /*error: invalid distance code (30-31 are never used)*/
        break;
      }
      distance = DISTANCEBASE[code_d];

      /*part 4: get extra bits from distance*/
      numextrabits_d = DISTANCEEXTRA[code_d];
      if(*bp >= inbitlength) ERROR_BREAK(51); /*error, bit pointer will jump past memory*/

      distance += readBitsFromStream(bp, in, numextrabits_d);

      /*part 5: fill in all the out[n] values based on the length and dist*/
      start = (*pos);
      if(distance > start) ERROR_BREAK(52); /*too long backward distance*/
      backward = start - distance;
      if((*pos) + length >= out->size)
      {
        /*reserve more room at once*/
        if(!ucvector_resize(out, ((*pos) + length) * 2)) ERROR_BREAK(83 /*alloc fail*/);
      }

      for(forward = 0; forward < length; forward++)
      {
        out->data[(*pos)] = out->data[backward];
        (*pos)++;
        backward++;
        if(backward >= start) backward = start - distance;
      }
    }
    else if(code_ll == 256)
    {
      break; /*end code, break the loop*/
    }
    else /*if(code == (unsigned)(-1))*/ /*huffmanDecodeSymbol returns (unsigned)(-1) in case of error*/
    {
      /*return error code 10 or 11 depending on the situation that happened in huffmanDecodeSymbol
      (10=no endcode, 11=wrong jump outside of tree)*/
      error = (*bp) > inlength * 8 ? 10 : 11;
      break;
    }
  }

  HuffmanTree_cleanup(&tree_ll);
  HuffmanTree_cleanup(&tree_d);

  return error;
}

static unsigned inflateNoCompression(ucvector* out, const unsigned char* in, size_t* bp, size_t* pos, size_t inlength)
{
  /*go to first boundary of byte*/
  size_t p;
  unsigned LEN, NLEN, n, error = 0;
  while(((*bp) & 0x7) != 0) (*bp)++;
  p = (*bp) / 8; /*byte position*/

  /*read LEN (2 bytes) and NLEN (2 bytes)*/
  if(p >= inlength - 4) return 52; /*error, bit pointer will jump past memory*/
  LEN = in[p] + 256 * in[p + 1]; p += 2;
  NLEN = in[p] + 256 * in[p + 1]; p += 2;

  /*check if 16-bit NLEN is really the one's complement of LEN*/
  if(LEN + NLEN != 65535) return 21; /*error: NLEN is not one's complement of LEN*/

  if((*pos) + LEN >= out->size)
  {
    if(!ucvector_resize(out, (*pos) + LEN)) return 83; /*alloc fail*/
  }

  /*read the literal data: LEN bytes are now stored in the out buffer*/
  if(p + LEN > inlength) return 23; /*error: reading outside of in buffer*/
  for(n = 0; n < LEN; n++) out->data[(*pos)++] = in[p++];

  (*bp) = p * 8;

  return error;
}

static unsigned lodepng_inflatev(ucvector* out,
                                 const unsigned char* in, size_t insize,
                                 const LodePNGDecompressSettings* settings)
{
  /*bit pointer in the "in" data, current byte is bp >> 3, current bit is bp & 0x7 (from lsb to msb of the byte)*/
  size_t bp = 0;
  unsigned BFINAL = 0;
  size_t pos = 0; /*byte position in the out buffer*/

  unsigned error = 0;

  (void)settings;

  while(!BFINAL)
  {
    unsigned BTYPE;
    if(bp + 2 >= insize * 8) return 52; /*error, bit pointer will jump past memory*/
    BFINAL = readBitFromStream(&bp, in);
    BTYPE = 1 * readBitFromStream(&bp, in);
    BTYPE += 2 * readBitFromStream(&bp, in);

    if(BTYPE == 3) return 20; /*error: invalid BTYPE*/
    else if(BTYPE == 0) error = inflateNoCompression(out, in, &bp, &pos, insize); /*no compression*/
    else error = inflateHuffmanBlock(out, in, &bp, &pos, insize, BTYPE); /*compression, BTYPE 01 or 10*/

    if(error) return error;
  }

  /*Only now we know the true size of out, resize it to that*/
  if(!ucvector_resize(out, pos)) error = 83; /*alloc fail*/

  return error;
}

unsigned lodepng_inflate(unsigned char** out, size_t* outsize,
                         const unsigned char* in, size_t insize,
                         const LodePNGDecompressSettings* settings)
{
  unsigned error;
  ucvector v;
  ucvector_init_buffer(&v, *out, *outsize);
  error = lodepng_inflatev(&v, in, insize, settings);
  *out = v.data;
  *outsize = v.size;
  return error;
}

static unsigned inflate(unsigned char** out, size_t* outsize,
                        const unsigned char* in, size_t insize,
                        const LodePNGDecompressSettings* settings)
{
  if(settings->custom_inflate)
  {
    return settings->custom_inflate(out, outsize, in, insize, settings);
  }
  else
  {
    return lodepng_inflate(out, outsize, in, insize, settings);
  }
}

#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER

/* ////////////////////////////////////////////////////////////////////////// */
/* / Deflator (Compressor)                                                  / */
/* ////////////////////////////////////////////////////////////////////////// */

static const size_t MAX_SUPPORTED_DEFLATE_LENGTH = 258;

/*bitlen is the size in bits of the code*/
static void addHuffmanSymbol(size_t* bp, ucvector* compressed, unsigned code, unsigned bitlen)
{
  addBitsToStreamReversed(bp, compressed, code, bitlen);
}

/*search the index in the array, that has the largest value smaller than or equal to the given value,
given array must be sorted (if no value is smaller, it returns the size of the given array)*/
static size_t searchCodeIndex(const unsigned* array, size_t array_size, size_t value)
{
  /*linear search implementation*/
  /*for(size_t i = 1; i < array_size; i++) if(array[i] > value) return i - 1;
  return array_size - 1;*/

  /*binary search implementation (not that much faster) (precondition: array_size > 0)*/
  size_t left  = 1;
  size_t right = array_size - 1;
  while(left <= right)
  {
    size_t mid = (left + right) / 2;
    if(array[mid] <= value) left = mid + 1; /*the value to find is more to the right*/
    else if(array[mid - 1] > value) right = mid - 1; /*the value to find is more to the left*/
    else return mid - 1;
  }
  return array_size - 1;
}

static void addLengthDistance(uivector* values, size_t length, size_t distance)
{
  /*values in encoded vector are those used by deflate:
  0-255: literal bytes
  256: end
  257-285: length/distance pair (length code, followed by extra length bits, distance code, extra distance bits)
  286-287: invalid*/

  unsigned length_code = (unsigned)searchCodeIndex(LENGTHBASE, 29, length);
  unsigned extra_length = (unsigned)(length - LENGTHBASE[length_code]);
  unsigned dist_code = (unsigned)searchCodeIndex(DISTANCEBASE, 30, distance);
  unsigned extra_distance = (unsigned)(distance - DISTANCEBASE[dist_code]);

  uivector_push_back(values, length_code + FIRST_LENGTH_CODE_INDEX);
  uivector_push_back(values, extra_length);
  uivector_push_back(values, dist_code);
  uivector_push_back(values, extra_distance);
}

static const unsigned HASH_NUM_VALUES = 65536;
static const unsigned HASH_NUM_CHARACTERS = 3;
static const unsigned HASH_SHIFT = 2;
/*
The HASH_NUM_CHARACTERS value is used to make encoding faster by using longer
sequences to generate a hash value from the stream bytes. Setting it to 3
gives exactly the same compression as the brute force method, since deflate's
run length encoding starts with lengths of 3. Setting it to higher values,
like 6, can make the encoding faster (not always though!), but will cause the
encoding to miss any length between 3 and this value, so that the compression
may be worse (but this can vary too depending on the image, sometimes it is
even a bit better instead).
The HASH_NUM_VALUES is the amount of unique possible hash values that
combinations of bytes can give, the higher it is the more memory is needed, but
if it's too low the advantage of hashing is gone.
*/

typedef struct Hash
{
  int* head; /*hash value to head circular pos*/
  int* val; /*circular pos to hash value*/
  /*circular pos to prev circular pos*/
  unsigned short* chain;
  unsigned short* zeros;
} Hash;

static unsigned hash_init(Hash* hash, unsigned windowsize)
{
  unsigned i;
  hash->head = (int*)lodepng_malloc(sizeof(int) * HASH_NUM_VALUES);
  hash->val = (int*)lodepng_malloc(sizeof(int) * windowsize);
  hash->chain = (unsigned short*)lodepng_malloc(sizeof(unsigned short) * windowsize);
  hash->zeros = (unsigned short*)lodepng_malloc(sizeof(unsigned short) * windowsize);

  if(!hash->head || !hash->val || !hash->chain || !hash->zeros) return 83; /*alloc fail*/

  /*initialize hash table*/
  for(i = 0; i < HASH_NUM_VALUES; i++) hash->head[i] = -1;
  for(i = 0; i < windowsize; i++) hash->val[i] = -1;
  for(i = 0; i < windowsize; i++) hash->chain[i] = i; /*same value as index indicates uninitialized*/

  return 0;
}

static void hash_cleanup(Hash* hash)
{
  lodepng_free(hash->head);
  lodepng_free(hash->val);
  lodepng_free(hash->chain);
  lodepng_free(hash->zeros);
}

static unsigned getHash(const unsigned char* data, size_t size, size_t pos)
{
  unsigned result = 0;
  size_t amount, i;
  if(pos >= size) return 0;
  amount = HASH_NUM_CHARACTERS;
  if(pos + amount >= size) amount = size - pos;
  for(i = 0; i < amount; i++) result ^= (data[pos + i] << (i * HASH_SHIFT));
  return result % HASH_NUM_VALUES;
}

static unsigned countZeros(const unsigned char* data, size_t size, size_t pos)
{
  const unsigned char* start = data + pos;
  const unsigned char* end = start + MAX_SUPPORTED_DEFLATE_LENGTH;
  if(end > data + size) end = data + size;
  data = start;
  while (data != end && *data == 0) data++;
  /*subtracting two addresses returned as 32-bit number (max value is MAX_SUPPORTED_DEFLATE_LENGTH)*/
  return (unsigned)(data - start);
}

static void updateHashChain(Hash* hash, size_t pos, int hashval, unsigned windowsize)
{
  unsigned wpos = pos % windowsize;
  hash->val[wpos] = hashval;
  if(hash->head[hashval] != -1) hash->chain[wpos] = hash->head[hashval];
  hash->head[hashval] = wpos;
}

/*
LZ77-encode the data. Return value is error code. The input are raw bytes, the output
is in the form of unsigned integers with codes representing for example literal bytes, or
length/distance pairs.
It uses a hash table technique to let it encode faster. When doing LZ77 encoding, a
sliding window (of windowsize) is used, and all past bytes in that window can be used as
the "dictionary". A brute force search through all possible distances would be slow, and
this hash technique is one out of several ways to speed this up.
*/
static unsigned encodeLZ77(uivector* out, Hash* hash,
                           const unsigned char* in, size_t inpos, size_t insize, unsigned windowsize,
                           unsigned minmatch, unsigned nicematch, unsigned lazymatching)
{
  unsigned short numzeros = 0;
  int usezeros = windowsize >= 8192; /*for small window size, the 'max chain length' optimization does a better job*/
  unsigned pos, i, error = 0;
  /*for large window lengths, assume the user wants no compression loss. Otherwise, max hash chain length speedup.*/
  unsigned maxchainlength = windowsize >= 8192 ? windowsize : windowsize / 8;
  unsigned maxlazymatch = windowsize >= 8192 ? MAX_SUPPORTED_DEFLATE_LENGTH : 64;

  if(!error)
  {
    unsigned offset; /*the offset represents the distance in LZ77 terminology*/
    unsigned length;
    unsigned lazy = 0;
    unsigned lazylength = 0, lazyoffset = 0;
    unsigned hashval;
    unsigned current_offset, current_length;
    const unsigned char *lastptr, *foreptr, *backptr;
    unsigned short hashpos, prevpos;

    for(pos = inpos; pos < insize; pos++)
    {
      size_t wpos = pos % windowsize; /*position for in 'circular' hash buffers*/

      hashval = getHash(in, insize, pos);
      updateHashChain(hash, pos, hashval, windowsize);

      if(usezeros && hashval == 0)
      {
        numzeros = countZeros(in, insize, pos);
        hash->zeros[wpos] = numzeros;
      }

      /*the length and offset found for the current position*/
      length = 0;
      offset = 0;

      prevpos = hash->head[hashval];
      hashpos = hash->chain[prevpos];

      lastptr = &in[insize < pos + MAX_SUPPORTED_DEFLATE_LENGTH ? insize : pos + MAX_SUPPORTED_DEFLATE_LENGTH];

      /*search for the longest string*/
      if(hash->val[wpos] == (int)hashval)
      {
        unsigned chainlength = 0;
        for(;;)
        {
          /*stop when went completely around the circular buffer*/
          if(prevpos < wpos && hashpos > prevpos && hashpos <= wpos) break;
          if(prevpos > wpos && (hashpos <= wpos || hashpos > prevpos)) break;
          if(chainlength++ >= maxchainlength) break;

          current_offset = hashpos <= wpos ? wpos - hashpos : wpos - hashpos + windowsize;
          if(current_offset > 0)
          {
            /*test the next characters*/
            foreptr = &in[pos];
            backptr = &in[pos - current_offset];

            /*common case in PNGs is lots of zeros. Quickly skip over them as a speedup*/
            if(usezeros && hashval == 0 && hash->val[hashpos] == 0 /*hashval[hashpos] may be out of date*/)
            {
              unsigned short skip = hash->zeros[hashpos];
              if(skip > numzeros) skip = numzeros;
              backptr += skip;
              foreptr += skip;
            }

            /* multiple checks at once per array bounds check */
            while(foreptr != lastptr && *backptr == *foreptr) /*maximum supported length by deflate is max length*/
            {
              ++backptr;
              ++foreptr;
            }
            current_length = (unsigned)(foreptr - &in[pos]);

            if(current_length > length)
            {
              length = current_length; /*the longest length*/
              offset = current_offset; /*the offset that is related to this longest length*/
              /*jump out once a length of max length is found (speed gain)*/
              if(current_length >= nicematch || current_length == MAX_SUPPORTED_DEFLATE_LENGTH) break;
            }
          }

          if(hashpos == hash->chain[hashpos]) break;

          prevpos = hashpos;
          hashpos = hash->chain[hashpos];
        }
      }

      if(lazymatching)
      {
        if(!lazy && length >= 3 && length <= maxlazymatch && length < MAX_SUPPORTED_DEFLATE_LENGTH)
        {
          lazy = 1;
          lazylength = length;
          lazyoffset = offset;
          continue; /*try the next byte*/
        }
        if(lazy)
        {
          lazy = 0;
          if(pos == 0) ERROR_BREAK(81);
          if(length > lazylength + 1)
          {
            /*push the previous character as literal*/
            if(!uivector_push_back(out, in[pos - 1])) ERROR_BREAK(83 /*alloc fail*/);
          }
          else
          {
            length = lazylength;
            offset = lazyoffset;
            hash->head[hashval] = -1; /*the same hashchain update will be done, this ensures no wrong alteration*/
            pos--;
          }
        }
      }
      if(length >= 3 && offset > windowsize) ERROR_BREAK(86 /*too big (or overflown negative) offset*/);

      /**encode it as length/distance pair or literal value**/
      if(length < 3) /*only lengths of 3 or higher are supported as length/distance pair*/
      {
        if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83 /*alloc fail*/);
      }
      else if(length < minmatch || (length == 3 && offset > 4096))
      {
        /*compensate for the fact that longer offsets have more extra bits, a
        length of only 3 may be not worth it then*/
        if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83 /*alloc fail*/);
      }
      else
      {
        addLengthDistance(out, length, offset);
        for(i = 1; i < length; i++)
        {
          pos++;
          hashval = getHash(in, insize, pos);
          updateHashChain(hash, pos, hashval, windowsize);
          if(usezeros && hashval == 0)
          {
            hash->zeros[pos % windowsize] = countZeros(in, insize, pos);
          }
        }
      }

    } /*end of the loop through each character of input*/
  } /*end of "if(!error)"*/

  return error;
}

/* /////////////////////////////////////////////////////////////////////////// */

static unsigned deflateNoCompression(ucvector* out, const unsigned char* data, size_t datasize)
{
  /*non compressed deflate block data: 1 bit BFINAL,2 bits BTYPE,(5 bits): it jumps to start of next byte,
  2 bytes LEN, 2 bytes NLEN, LEN bytes literal DATA*/

  size_t i, j, numdeflateblocks = (datasize + 65534) / 65535;
  unsigned datapos = 0;
  for(i = 0; i < numdeflateblocks; i++)
  {
    unsigned BFINAL, BTYPE, LEN, NLEN;
    unsigned char firstbyte;

    BFINAL = (i == numdeflateblocks - 1);
    BTYPE = 0;

    firstbyte = (unsigned char)(BFINAL + ((BTYPE & 1) << 1) + ((BTYPE & 2) << 1));
    ucvector_push_back(out, firstbyte);

    LEN = 65535;
    if(datasize - datapos < 65535) LEN = (unsigned)datasize - datapos;
    NLEN = 65535 - LEN;

    ucvector_push_back(out, (unsigned char)(LEN % 256));
    ucvector_push_back(out, (unsigned char)(LEN / 256));
    ucvector_push_back(out, (unsigned char)(NLEN % 256));
    ucvector_push_back(out, (unsigned char)(NLEN / 256));

    /*Decompressed data*/
    for(j = 0; j < 65535 && datapos < datasize; j++)
    {
      ucvector_push_back(out, data[datapos++]);
    }
  }

  return 0;
}

/*
write the lz77-encoded data, which has lit, len and dist codes, to compressed stream using huffman trees.
tree_ll: the tree for lit and len codes.
tree_d: the tree for distance codes.
*/
static void writeLZ77data(size_t* bp, ucvector* out, const uivector* lz77_encoded,
                          const HuffmanTree* tree_ll, const HuffmanTree* tree_d)
{
  size_t i = 0;
  for(i = 0; i < lz77_encoded->size; i++)
  {
    unsigned val = lz77_encoded->data[i];
    addHuffmanSymbol(bp, out, HuffmanTree_getCode(tree_ll, val), HuffmanTree_getLength(tree_ll, val));
    if(val > 256) /*for a length code, 3 more things have to be added*/
    {
      unsigned length_index = val - FIRST_LENGTH_CODE_INDEX;
      unsigned n_length_extra_bits = LENGTHEXTRA[length_index];
      unsigned length_extra_bits = lz77_encoded->data[++i];

      unsigned distance_code = lz77_encoded->data[++i];

      unsigned distance_index = distance_code;
      unsigned n_distance_extra_bits = DISTANCEEXTRA[distance_index];
      unsigned distance_extra_bits = lz77_encoded->data[++i];

      addBitsToStream(bp, out, length_extra_bits, n_length_extra_bits);
      addHuffmanSymbol(bp, out, HuffmanTree_getCode(tree_d, distance_code),
                       HuffmanTree_getLength(tree_d, distance_code));
      addBitsToStream(bp, out, distance_extra_bits, n_distance_extra_bits);
    }
  }
}

/*Deflate for a block of type "dynamic", that is, with freely, optimally, created huffman trees*/
static unsigned deflateDynamic(ucvector* out, size_t* bp, Hash* hash,
                               const unsigned char* data, size_t datapos, size_t dataend,
                               const LodePNGCompressSettings* settings, int final)
{
  unsigned error = 0;

  /*
  A block is compressed as follows: The PNG data is lz77 encoded, resulting in
  literal bytes and length/distance pairs. This is then huffman compressed with
  two huffman trees. One huffman tree is used for the lit and len values ("ll"),
  another huffman tree is used for the dist values ("d"). These two trees are
  stored using their code lengths, and to compress even more these code lengths
  are also run-length encoded and huffman compressed. This gives a huffman tree
  of code lengths "cl". The code lenghts used to describe this third tree are
  the code length code lengths ("clcl").
  */

  /*The lz77 encoded data, represented with integers since there will also be length and distance codes in it*/
  uivector lz77_encoded;
  HuffmanTree tree_ll; /*tree for lit,len values*/
  HuffmanTree tree_d; /*tree for distance codes*/
  HuffmanTree tree_cl; /*tree for encoding the code lengths representing tree_ll and tree_d*/
  uivector frequencies_ll; /*frequency of lit,len codes*/
  uivector frequencies_d; /*frequency of dist codes*/
  uivector frequencies_cl; /*frequency of code length codes*/
  uivector bitlen_lld; /*lit,len,dist code lenghts (int bits), literally (without repeat codes).*/
  uivector bitlen_lld_e; /*bitlen_lld encoded with repeat codes (this is a rudemtary run length compression)*/
  /*bitlen_cl is the code length code lengths ("clcl"). The bit lengths of codes to represent tree_cl
  (these are written as is in the file, it would be crazy to compress these using yet another huffman
  tree that needs to be represented by yet another set of code lengths)*/
  uivector bitlen_cl;
  size_t datasize = dataend - datapos;

  /*
  Due to the huffman compression of huffman tree representations ("two levels"), there are some anologies:
  bitlen_lld is to tree_cl what data is to tree_ll and tree_d.
  bitlen_lld_e is to bitlen_lld what lz77_encoded is to data.
  bitlen_cl is to bitlen_lld_e what bitlen_lld is to lz77_encoded.
  */

  unsigned BFINAL = final;
  size_t numcodes_ll, numcodes_d, i;
  unsigned HLIT, HDIST, HCLEN;

  uivector_init(&lz77_encoded);
  HuffmanTree_init(&tree_ll);
  HuffmanTree_init(&tree_d);
  HuffmanTree_init(&tree_cl);
  uivector_init(&frequencies_ll);
  uivector_init(&frequencies_d);
  uivector_init(&frequencies_cl);
  uivector_init(&bitlen_lld);
  uivector_init(&bitlen_lld_e);
  uivector_init(&bitlen_cl);

  /*This while loop never loops due to a break at the end, it is here to
  allow breaking out of it to the cleanup phase on error conditions.*/
  while(!error)
  {
    if(settings->use_lz77)
    {
      error = encodeLZ77(&lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
                         settings->minmatch, settings->nicematch, settings->lazymatching);
      if(error) break;
    }
    else
    {
      if(!uivector_resize(&lz77_encoded, datasize)) ERROR_BREAK(83 /*alloc fail*/);
      for(i = datapos; i < dataend; i++) lz77_encoded.data[i] = data[i]; /*no LZ77, but still will be Huffman compressed*/
    }

    if(!uivector_resizev(&frequencies_ll, 286, 0)) ERROR_BREAK(83 /*alloc fail*/);
    if(!uivector_resizev(&frequencies_d, 30, 0)) ERROR_BREAK(83 /*alloc fail*/);

    /*Count the frequencies of lit, len and dist codes*/
    for(i = 0; i < lz77_encoded.size; i++)
    {
      unsigned symbol = lz77_encoded.data[i];
      frequencies_ll.data[symbol]++;
      if(symbol > 256)
      {
        unsigned dist = lz77_encoded.data[i + 2];
        frequencies_d.data[dist]++;
        i += 3;
      }
    }
    frequencies_ll.data[256] = 1; /*there will be exactly 1 end code, at the end of the block*/

    /*Make both huffman trees, one for the lit and len codes, one for the dist codes*/
    error = HuffmanTree_makeFromFrequencies(&tree_ll, frequencies_ll.data, 257, frequencies_ll.size, 15);
    if(error) break;
    /*2, not 1, is chosen for mincodes: some buggy PNG decoders require at least 2 symbols in the dist tree*/
    error = HuffmanTree_makeFromFrequencies(&tree_d, frequencies_d.data, 2, frequencies_d.size, 15);
    if(error) break;

    numcodes_ll = tree_ll.numcodes; if(numcodes_ll > 286) numcodes_ll = 286;
    numcodes_d = tree_d.numcodes; if(numcodes_d > 30) numcodes_d = 30;
    /*store the code lengths of both generated trees in bitlen_lld*/
    for(i = 0; i < numcodes_ll; i++) uivector_push_back(&bitlen_lld, HuffmanTree_getLength(&tree_ll, (unsigned)i));
    for(i = 0; i < numcodes_d; i++) uivector_push_back(&bitlen_lld, HuffmanTree_getLength(&tree_d, (unsigned)i));

    /*run-length compress bitlen_ldd into bitlen_lld_e by using repeat codes 16 (copy length 3-6 times),
    17 (3-10 zeroes), 18 (11-138 zeroes)*/
    for(i = 0; i < (unsigned)bitlen_lld.size; i++)
    {
      unsigned j = 0; /*amount of repititions*/
      while(i + j + 1 < (unsigned)bitlen_lld.size && bitlen_lld.data[i + j + 1] == bitlen_lld.data[i]) j++;

      if(bitlen_lld.data[i] == 0 && j >= 2) /*repeat code for zeroes*/
      {
        j++; /*include the first zero*/
        if(j <= 10) /*repeat code 17 supports max 10 zeroes*/
        {
          uivector_push_back(&bitlen_lld_e, 17);
          uivector_push_back(&bitlen_lld_e, j - 3);
        }
        else /*repeat code 18 supports max 138 zeroes*/
        {
          if(j > 138) j = 138;
          uivector_push_back(&bitlen_lld_e, 18);
          uivector_push_back(&bitlen_lld_e, j - 11);
        }
        i += (j - 1);
      }
      else if(j >= 3) /*repeat code for value other than zero*/
      {
        size_t k;
        unsigned num = j / 6, rest = j % 6;
        uivector_push_back(&bitlen_lld_e, bitlen_lld.data[i]);
        for(k = 0; k < num; k++)
        {
          uivector_push_back(&bitlen_lld_e, 16);
          uivector_push_back(&bitlen_lld_e, 6 - 3);
        }
        if(rest >= 3)
        {
          uivector_push_back(&bitlen_lld_e, 16);
          uivector_push_back(&bitlen_lld_e, rest - 3);
        }
        else j -= rest;
        i += j;
      }
      else /*too short to benefit from repeat code*/
      {
        uivector_push_back(&bitlen_lld_e, bitlen_lld.data[i]);
      }
    }

    /*generate tree_cl, the huffmantree of huffmantrees*/

    if(!uivector_resizev(&frequencies_cl, NUM_CODE_LENGTH_CODES, 0)) ERROR_BREAK(83 /*alloc fail*/);
    for(i = 0; i < bitlen_lld_e.size; i++)
    {
      frequencies_cl.data[bitlen_lld_e.data[i]]++;
      /*after a repeat code come the bits that specify the number of repetitions,
      those don't need to be in the frequencies_cl calculation*/
      if(bitlen_lld_e.data[i] >= 16) i++;
    }

    error = HuffmanTree_makeFromFrequencies(&tree_cl, frequencies_cl.data,
                                            frequencies_cl.size, frequencies_cl.size, 7);
    if(error) break;

    if(!uivector_resize(&bitlen_cl, tree_cl.numcodes)) ERROR_BREAK(83 /*alloc fail*/);
    for(i = 0; i < tree_cl.numcodes; i++)
    {
      /*lenghts of code length tree is in the order as specified by deflate*/
      bitlen_cl.data[i] = HuffmanTree_getLength(&tree_cl, CLCL_ORDER[i]);
    }
    while(bitlen_cl.data[bitlen_cl.size - 1] == 0 && bitlen_cl.size > 4)
    {
      /*remove zeros at the end, but minimum size must be 4*/
      if(!uivector_resize(&bitlen_cl, bitlen_cl.size - 1)) ERROR_BREAK(83 /*alloc fail*/);
    }
    if(error) break;

    /*
    Write everything into the output

    After the BFINAL and BTYPE, the dynamic block consists out of the following:
    - 5 bits HLIT, 5 bits HDIST, 4 bits HCLEN
    - (HCLEN+4)*3 bits code lengths of code length alphabet
    - HLIT + 257 code lenghts of lit/length alphabet (encoded using the code length
      alphabet, + possible repetition codes 16, 17, 18)
    - HDIST + 1 code lengths of distance alphabet (encoded using the code length
      alphabet, + possible repetition codes 16, 17, 18)
    - compressed data
    - 256 (end code)
    */

    /*Write block type*/
    addBitToStream(bp, out, BFINAL);
    addBitToStream(bp, out, 0); /*first bit of BTYPE "dynamic"*/
    addBitToStream(bp, out, 1); /*second bit of BTYPE "dynamic"*/

    /*write the HLIT, HDIST and HCLEN values*/
    HLIT = (unsigned)(numcodes_ll - 257);
    HDIST = (unsigned)(numcodes_d - 1);
    HCLEN = (unsigned)bitlen_cl.size - 4;
    /*trim zeroes for HCLEN. HLIT and HDIST were already trimmed at tree creation*/
    while(!bitlen_cl.data[HCLEN + 4 - 1] && HCLEN > 0) HCLEN--;
    addBitsToStream(bp, out, HLIT, 5);
    addBitsToStream(bp, out, HDIST, 5);
    addBitsToStream(bp, out, HCLEN, 4);

    /*write the code lenghts of the code length alphabet*/
    for(i = 0; i < HCLEN + 4; i++) addBitsToStream(bp, out, bitlen_cl.data[i], 3);

    /*write the lenghts of the lit/len AND the dist alphabet*/
    for(i = 0; i < bitlen_lld_e.size; i++)
    {
      addHuffmanSymbol(bp, out, HuffmanTree_getCode(&tree_cl, bitlen_lld_e.data[i]),
                       HuffmanTree_getLength(&tree_cl, bitlen_lld_e.data[i]));
      /*extra bits of repeat codes*/
      if(bitlen_lld_e.data[i] == 16) addBitsToStream(bp, out, bitlen_lld_e.data[++i], 2);
      else if(bitlen_lld_e.data[i] == 17) addBitsToStream(bp, out, bitlen_lld_e.data[++i], 3);
      else if(bitlen_lld_e.data[i] == 18) addBitsToStream(bp, out, bitlen_lld_e.data[++i], 7);
    }

    /*write the compressed data symbols*/
    writeLZ77data(bp, out, &lz77_encoded, &tree_ll, &tree_d);
    /*error: the length of the end code 256 must be larger than 0*/
    if(HuffmanTree_getLength(&tree_ll, 256) == 0) ERROR_BREAK(64);

    /*write the end code*/
    addHuffmanSymbol(bp, out, HuffmanTree_getCode(&tree_ll, 256), HuffmanTree_getLength(&tree_ll, 256));

    break; /*end of error-while*/
  }

  /*cleanup*/
  uivector_cleanup(&lz77_encoded);
  HuffmanTree_cleanup(&tree_ll);
  HuffmanTree_cleanup(&tree_d);
  HuffmanTree_cleanup(&tree_cl);
  uivector_cleanup(&frequencies_ll);
  uivector_cleanup(&frequencies_d);
  uivector_cleanup(&frequencies_cl);
  uivector_cleanup(&bitlen_lld_e);
  uivector_cleanup(&bitlen_lld);
  uivector_cleanup(&bitlen_cl);

  return error;
}

static unsigned deflateFixed(ucvector* out, size_t* bp, Hash* hash,
                             const unsigned char* data,
                             size_t datapos, size_t dataend,
                             const LodePNGCompressSettings* settings, int final)
{
  HuffmanTree tree_ll; /*tree for literal values and length codes*/
  HuffmanTree tree_d; /*tree for distance codes*/

  unsigned BFINAL = final;
  unsigned error = 0;
  size_t i;

  HuffmanTree_init(&tree_ll);
  HuffmanTree_init(&tree_d);

  generateFixedLitLenTree(&tree_ll);
  generateFixedDistanceTree(&tree_d);

  addBitToStream(bp, out, BFINAL);
  addBitToStream(bp, out, 1); /*first bit of BTYPE*/
  addBitToStream(bp, out, 0); /*second bit of BTYPE*/

  if(settings->use_lz77) /*LZ77 encoded*/
  {
    uivector lz77_encoded;
    uivector_init(&lz77_encoded);
    error = encodeLZ77(&lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
                       settings->minmatch, settings->nicematch, settings->lazymatching);
    if(!error) writeLZ77data(bp, out, &lz77_encoded, &tree_ll, &tree_d);
    uivector_cleanup(&lz77_encoded);
  }
  else /*no LZ77, but still will be Huffman compressed*/
  {
    for(i = datapos; i < dataend; i++)
    {
      addHuffmanSymbol(bp, out, HuffmanTree_getCode(&tree_ll, data[i]), HuffmanTree_getLength(&tree_ll, data[i]));
    }
  }
  /*add END code*/
  if(!error) addHuffmanSymbol(bp, out, HuffmanTree_getCode(&tree_ll, 256), HuffmanTree_getLength(&tree_ll, 256));

  /*cleanup*/
  HuffmanTree_cleanup(&tree_ll);
  HuffmanTree_cleanup(&tree_d);

  return error;
}

static unsigned lodepng_deflatev(ucvector* out, const unsigned char* in, size_t insize,
                                 const LodePNGCompressSettings* settings)
{
  unsigned error = 0;
  size_t i, blocksize, numdeflateblocks;
  size_t bp = 0; /*the bit pointer*/
  Hash hash;

  if(settings->btype > 2) return 61;
  else if(settings->btype == 0) return deflateNoCompression(out, in, insize);
  else if(settings->btype == 1) blocksize = insize;
  else /*if(settings->btype == 2)*/
  {
    blocksize = insize / 8 + 8;
    if(blocksize < 65535) blocksize = 65535;
  }

  numdeflateblocks = (insize + blocksize - 1) / blocksize;
  if(numdeflateblocks == 0) numdeflateblocks = 1;

  error = hash_init(&hash, settings->windowsize);
  if(error) return error;

  for(i = 0; i < numdeflateblocks && !error; i++)
  {
    int final = i == numdeflateblocks - 1;
    size_t start = i * blocksize;
    size_t end = start + blocksize;
    if(end > insize) end = insize;

    if(settings->btype == 1) error = deflateFixed(out, &bp, &hash, in, start, end, settings, final);
    else if(settings->btype == 2) error = deflateDynamic(out, &bp, &hash, in, start, end, settings, final);
  }

  hash_cleanup(&hash);

  return error;
}

unsigned lodepng_deflate(unsigned char** out, size_t* outsize,
                         const unsigned char* in, size_t insize,
                         const LodePNGCompressSettings* settings)
{
  unsigned error;
  ucvector v;
  ucvector_init_buffer(&v, *out, *outsize);
  error = lodepng_deflatev(&v, in, insize, settings);
  *out = v.data;
  *outsize = v.size;
  return error;
}

static unsigned deflate(unsigned char** out, size_t* outsize,
                        const unsigned char* in, size_t insize,
                        const LodePNGCompressSettings* settings)
{
  if(settings->custom_deflate)
  {
    return settings->custom_deflate(out, outsize, in, insize, settings);
  }
  else
  {
    return lodepng_deflate(out, outsize, in, insize, settings);
  }
}

#endif /*LODEPNG_COMPILE_DECODER*/

/* ////////////////////////////////////////////////////////////////////////// */
/* / Adler32                                                                  */
/* ////////////////////////////////////////////////////////////////////////// */

static unsigned update_adler32(unsigned adler, const unsigned char* data, unsigned len)
{
   unsigned s1 = adler & 0xffff;
   unsigned s2 = (adler >> 16) & 0xffff;

  while(len > 0)
  {
    /*at least 5550 sums can be done before the sums overflow, saving a lot of module divisions*/
    unsigned amount = len > 5550 ? 5550 : len;
    len -= amount;
    while(amount > 0)
    {
      s1 += (*data++);
      s2 += s1;
      amount--;
    }
    s1 %= 65521;
    s2 %= 65521;
  }

  return (s2 << 16) | s1;
}

/*Return the adler32 of the bytes data[0..len-1]*/
static unsigned adler32(const unsigned char* data, unsigned len)
{
  return update_adler32(1L, data, len);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* / Zlib                                                                   / */
/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_DECODER

unsigned lodepng_zlib_decompress(unsigned char** out, size_t* outsize, const unsigned char* in,
                                 size_t insize, const LodePNGDecompressSettings* settings)
{
  unsigned error = 0;
  unsigned CM, CINFO, FDICT;

  if(insize < 2) return 53; /*error, size of zlib data too small*/
  /*read information from zlib header*/
  if((in[0] * 256 + in[1]) % 31 != 0)
  {
    /*error: 256 * in[0] + in[1] must be a multiple of 31, the FCHECK value is supposed to be made that way*/
    return 24;
  }

  CM = in[0] & 15;
  CINFO = (in[0] >> 4) & 15;
  /*FCHECK = in[1] & 31;*/ /*FCHECK is already tested above*/
  FDICT = (in[1] >> 5) & 1;
  /*FLEVEL = (in[1] >> 6) & 3;*/ /*FLEVEL is not used here*/

  if(CM != 8 || CINFO > 7)
  {
    /*error: only compression method 8: inflate with sliding window of 32k is supported by the PNG spec*/
    return 25;
  }
  if(FDICT != 0)
  {
    /*error: the specification of PNG says about the zlib stream:
      "The additional flags shall not specify a preset dictionary."*/
    return 26;
  }

  error = inflate(out, outsize, in + 2, insize - 2, settings);
  if(error) return error;

  if(!settings->ignore_adler32)
  {
    unsigned ADLER32 = lodepng_read32bitInt(&in[insize - 4]);
    unsigned checksum = adler32(*out, (unsigned)(*outsize));
    if(checksum != ADLER32) return 58; /*error, adler checksum not correct, data must be corrupted*/
  }

  return 0; /*no error*/
}

static unsigned zlib_decompress(unsigned char** out, size_t* outsize, const unsigned char* in,
                                size_t insize, const LodePNGDecompressSettings* settings)
{
  if(settings->custom_zlib)
    return settings->custom_zlib(out, outsize, in, insize, settings);
  else
    return lodepng_zlib_decompress(out, outsize, in, insize, settings);
}

#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER

unsigned lodepng_zlib_compress(unsigned char** out, size_t* outsize, const unsigned char* in,
                               size_t insize, const LodePNGCompressSettings* settings)
{
  /*initially, *out must be NULL and outsize 0, if you just give some random *out
  that's pointing to a non allocated buffer, this'll crash*/
  ucvector outv;
  size_t i;
  unsigned error;
  unsigned char* deflatedata = 0;
  size_t deflatesize = 0;

  unsigned ADLER32;
  /*zlib data: 1 byte CMF (CM+CINFO), 1 byte FLG, deflate data, 4 byte ADLER32 checksum of the Decompressed data*/
  unsigned CMF = 120; /*0b01111000: CM 8, CINFO 7. With CINFO 7, any window size up to 32768 can be used.*/
  unsigned FLEVEL = 0;
  unsigned FDICT = 0;
  unsigned CMFFLG = 256 * CMF + FDICT * 32 + FLEVEL * 64;
  unsigned FCHECK = 31 - CMFFLG % 31;
  CMFFLG += FCHECK;

  /*ucvector-controlled version of the output buffer, for dynamic array*/
  ucvector_init_buffer(&outv, *out, *outsize);

  ucvector_push_back(&outv, (unsigned char)(CMFFLG / 256));
  ucvector_push_back(&outv, (unsigned char)(CMFFLG % 256));

  error = deflate(&deflatedata, &deflatesize, in, insize, settings);

  if(!error)
  {
    ADLER32 = adler32(in, (unsigned)insize);
    for(i = 0; i < deflatesize; i++) ucvector_push_back(&outv, deflatedata[i]);
    lodepng_free(deflatedata);
    lodepng_add32bitInt(&outv, ADLER32);
  }

  *out = outv.data;
  *outsize = outv.size;

  return error;
}

/* compress using the default or custom zlib function */
static unsigned zlib_compress(unsigned char** out, size_t* outsize, const unsigned char* in,
                              size_t insize, const LodePNGCompressSettings* settings)
{
  if(settings->custom_zlib)
  {
    return settings->custom_zlib(out, outsize, in, insize, settings);
  }
  else
  {
    return lodepng_zlib_compress(out, outsize, in, insize, settings);
  }
}

#endif /*LODEPNG_COMPILE_ENCODER*/

#else /*no LODEPNG_COMPILE_ZLIB*/

#ifdef LODEPNG_COMPILE_DECODER
static unsigned zlib_decompress(unsigned char** out, size_t* outsize, const unsigned char* in,
                                size_t insize, const LodePNGDecompressSettings* settings)
{
  if (!settings->custom_zlib) return 87; /*no custom zlib function provided */
  return settings->custom_zlib(out, outsize, in, insize, settings);
}
#endif /*LODEPNG_COMPILE_DECODER*/
#ifdef LODEPNG_COMPILE_ENCODER
static unsigned zlib_compress(unsigned char** out, size_t* outsize, const unsigned char* in,
                              size_t insize, const LodePNGCompressSettings* settings)
{
  if (!settings->custom_zlib) return 87; /*no custom zlib function provided */
  return settings->custom_zlib(out, outsize, in, insize, settings);
}
#endif /*LODEPNG_COMPILE_ENCODER*/

#endif /*LODEPNG_COMPILE_ZLIB*/

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_ENCODER

/*this is a good tradeoff between speed and compression ratio*/
#define DEFAULT_WINDOWSIZE 2048

void lodepng_compress_settings_init(LodePNGCompressSettings* settings)
{
  /*compress with dynamic huffman tree (not in the mathematical sense, just not the predefined one)*/
  settings->btype = 2;
  settings->use_lz77 = 1;
  settings->windowsize = DEFAULT_WINDOWSIZE;
  settings->minmatch = 3;
  settings->nicematch = 128;
  settings->lazymatching = 1;

  settings->custom_zlib = 0;
  settings->custom_deflate = 0;
  settings->custom_context = 0;
}

const LodePNGCompressSettings lodepng_default_compress_settings = {2, 1, DEFAULT_WINDOWSIZE, 3, 128, 1, 0, 0, 0};


#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_DECODER

void lodepng_decompress_settings_init(LodePNGDecompressSettings* settings)
{
  settings->ignore_adler32 = 0;

  settings->custom_zlib = 0;
  settings->custom_inflate = 0;
  settings->custom_context = 0;
}

const LodePNGDecompressSettings lodepng_default_decompress_settings = {0, 0, 0, 0};

#endif /*LODEPNG_COMPILE_DECODER*/

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* // End of Zlib related code. Begin of PNG related code.                 // */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */

#ifdef LODEPNG_COMPILE_PNG

/* ////////////////////////////////////////////////////////////////////////// */
/* / CRC32                                                                  / */
/* ////////////////////////////////////////////////////////////////////////// */

/* CRC polynomial: 0xedb88320 */
static unsigned lodepng_crc32_table[256] = {
           0u, 1996959894u, 3993919788u, 2567524794u,  124634137u, 1886057615u, 3915621685u, 2657392035u,
   249268274u, 2044508324u, 3772115230u, 2547177864u,  162941995u, 2125561021u, 3887607047u, 2428444049u,
   498536548u, 1789927666u, 4089016648u, 2227061214u,  450548861u, 1843258603u, 4107580753u, 2211677639u,
   325883990u, 1684777152u, 4251122042u, 2321926636u,  335633487u, 1661365465u, 4195302755u, 2366115317u,
   997073096u, 1281953886u, 3579855332u, 2724688242u, 1006888145u, 1258607687u, 3524101629u, 2768942443u,
   901097722u, 1119000684u, 3686517206u, 2898065728u,  853044451u, 1172266101u, 3705015759u, 2882616665u,
   651767980u, 1373503546u, 3369554304u, 3218104598u,  565507253u, 1454621731u, 3485111705u, 3099436303u,
   671266974u, 1594198024u, 3322730930u, 2970347812u,  795835527u, 1483230225u, 3244367275u, 3060149565u,
  1994146192u,   31158534u, 2563907772u, 4023717930u, 1907459465u,  112637215u, 2680153253u, 3904427059u,
  2013776290u,  251722036u, 2517215374u, 3775830040u, 2137656763u,  141376813u, 2439277719u, 3865271297u,
  1802195444u,  476864866u, 2238001368u, 4066508878u, 1812370925u,  453092731u, 2181625025u, 4111451223u,
  1706088902u,  314042704u, 2344532202u, 4240017532u, 1658658271u,  366619977u, 2362670323u, 4224994405u,
  1303535960u,  984961486u, 2747007092u, 3569037538u, 1256170817u, 1037604311u, 2765210733u, 3554079995u,
  1131014506u,  879679996u, 2909243462u, 3663771856u, 1141124467u,  855842277u, 2852801631u, 3708648649u,
  1342533948u,  654459306u, 3188396048u, 3373015174u, 1466479909u,  544179635u, 3110523913u, 3462522015u,
  1591671054u,  702138776u, 2966460450u, 3352799412u, 1504918807u,  783551873u, 3082640443u, 3233442989u,
  3988292384u, 2596254646u,   62317068u, 1957810842u, 3939845945u, 2647816111u,   81470997u, 1943803523u,
  3814918930u, 2489596804u,  225274430u, 2053790376u, 3826175755u, 2466906013u,  167816743u, 2097651377u,
  4027552580u, 2265490386u,  503444072u, 1762050814u, 4150417245u, 2154129355u,  426522225u, 1852507879u,
  4275313526u, 2312317920u,  282753626u, 1742555852u, 4189708143u, 2394877945u,  397917763u, 1622183637u,
  3604390888u, 2714866558u,  953729732u, 1340076626u, 3518719985u, 2797360999u, 1068828381u, 1219638859u,
  3624741850u, 2936675148u,  906185462u, 1090812512u, 3747672003u, 2825379669u,  829329135u, 1181335161u,
  3412177804u, 3160834842u,  628085408u, 1382605366u, 3423369109u, 3138078467u,  570562233u, 1426400815u,
  3317316542u, 2998733608u,  733239954u, 1555261956u, 3268935591u, 3050360625u,  752459403u, 1541320221u,
  2607071920u, 3965973030u, 1969922972u,   40735498u, 2617837225u, 3943577151u, 1913087877u,   83908371u,
  2512341634u, 3803740692u, 2075208622u,  213261112u, 2463272603u, 3855990285u, 2094854071u,  198958881u,
  2262029012u, 4057260610u, 1759359992u,  534414190u, 2176718541u, 4139329115u, 1873836001u,  414664567u,
  2282248934u, 4279200368u, 1711684554u,  285281116u, 2405801727u, 4167216745u, 1634467795u,  376229701u,
  2685067896u, 3608007406u, 1308918612u,  956543938u, 2808555105u, 3495958263u, 1231636301u, 1047427035u,
  2932959818u, 3654703836u, 1088359270u,  936918000u, 2847714899u, 3736837829u, 1202900863u,  817233897u,
  3183342108u, 3401237130u, 1404277552u,  615818150u, 3134207493u, 3453421203u, 1423857449u,  601450431u,
  3009837614u, 3294710456u, 1567103746u,  711928724u, 3020668471u, 3272380065u, 1510334235u,  755167117u
};

/*Return the CRC of the bytes buf[0..len-1].*/
unsigned lodepng_crc32(const unsigned char* buf, size_t len)
{
  unsigned c = 0xffffffffL;
  size_t n;

  for(n = 0; n < len; n++)
  {
    c = lodepng_crc32_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
  }
  return c ^ 0xffffffffL;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* / Reading and writing single bits and bytes from/to stream for LodePNG   / */
/* ////////////////////////////////////////////////////////////////////////// */

static unsigned char readBitFromReversedStream(size_t* bitpointer, const unsigned char* bitstream)
{
  unsigned char result = (unsigned char)((bitstream[(*bitpointer) >> 3] >> (7 - ((*bitpointer) & 0x7))) & 1);
  (*bitpointer)++;
  return result;
}

static unsigned readBitsFromReversedStream(size_t* bitpointer, const unsigned char* bitstream, size_t nbits)
{
  unsigned result = 0;
  size_t i;
  for(i = nbits - 1; i < nbits; i--)
  {
    result += (unsigned)readBitFromReversedStream(bitpointer, bitstream) << i;
  }
  return result;
}

#ifdef LODEPNG_COMPILE_DECODER
static void setBitOfReversedStream0(size_t* bitpointer, unsigned char* bitstream, unsigned char bit)
{
  /*the current bit in bitstream must be 0 for this to work*/
  if(bit)
  {
    /*earlier bit of huffman code is in a lesser significant bit of an earlier byte*/
    bitstream[(*bitpointer) >> 3] |= (bit << (7 - ((*bitpointer) & 0x7)));
  }
  (*bitpointer)++;
}
#endif /*LODEPNG_COMPILE_DECODER*/

static void setBitOfReversedStream(size_t* bitpointer, unsigned char* bitstream, unsigned char bit)
{
  /*the current bit in bitstream may be 0 or 1 for this to work*/
  if(bit == 0) bitstream[(*bitpointer) >> 3] &=  (unsigned char)(~(1 << (7 - ((*bitpointer) & 0x7))));
  else         bitstream[(*bitpointer) >> 3] |=  (1 << (7 - ((*bitpointer) & 0x7)));
  (*bitpointer)++;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* / PNG chunks                                                             / */
/* ////////////////////////////////////////////////////////////////////////// */

unsigned lodepng_chunk_length(const unsigned char* chunk)
{
  return lodepng_read32bitInt(&chunk[0]);
}

void lodepng_chunk_type(char type[5], const unsigned char* chunk)
{
  unsigned i;
  for(i = 0; i < 4; i++) type[i] = chunk[4 + i];
  type[4] = 0; /*null termination char*/
}

unsigned char lodepng_chunk_type_equals(const unsigned char* chunk, const char* type)
{
  if(strlen(type) != 4) return 0;
  return (chunk[4] == type[0] && chunk[5] == type[1] && chunk[6] == type[2] && chunk[7] == type[3]);
}

unsigned char lodepng_chunk_ancillary(const unsigned char* chunk)
{
  return((chunk[4] & 32) != 0);
}

unsigned char lodepng_chunk_private(const unsigned char* chunk)
{
  return((chunk[6] & 32) != 0);
}

unsigned char lodepng_chunk_safetocopy(const unsigned char* chunk)
{
  return((chunk[7] & 32) != 0);
}

unsigned char* lodepng_chunk_data(unsigned char* chunk)
{
  return &chunk[8];
}

const unsigned char* lodepng_chunk_data_const(const unsigned char* chunk)
{
  return &chunk[8];
}

unsigned lodepng_chunk_check_crc(const unsigned char* chunk)
{
  unsigned length = lodepng_chunk_length(chunk);
  unsigned CRC = lodepng_read32bitInt(&chunk[length + 8]);
  /*the CRC is taken of the data and the 4 chunk type letters, not the length*/
  unsigned checksum = lodepng_crc32(&chunk[4], length + 4);
  if(CRC != checksum) return 1;
  else return 0;
}

void lodepng_chunk_generate_crc(unsigned char* chunk)
{
  unsigned length = lodepng_chunk_length(chunk);
  unsigned CRC = lodepng_crc32(&chunk[4], length + 4);
  lodepng_set32bitInt(chunk + 8 + length, CRC);
}

unsigned char* lodepng_chunk_next(unsigned char* chunk)
{
  unsigned total_chunk_length = lodepng_chunk_length(chunk) + 12;
  return &chunk[total_chunk_length];
}

const unsigned char* lodepng_chunk_next_const(const unsigned char* chunk)
{
  unsigned total_chunk_length = lodepng_chunk_length(chunk) + 12;
  return &chunk[total_chunk_length];
}

unsigned lodepng_chunk_append(unsigned char** out, size_t* outlength, const unsigned char* chunk)
{
  unsigned i;
  unsigned total_chunk_length = lodepng_chunk_length(chunk) + 12;
  unsigned char *chunk_start, *new_buffer;
  size_t new_length = (*outlength) + total_chunk_length;
  if(new_length < total_chunk_length || new_length < (*outlength)) return 77; /*integer overflow happened*/

  new_buffer = (unsigned char*)lodepng_realloc(*out, new_length);
  if(!new_buffer) return 83; /*alloc fail*/
  (*out) = new_buffer;
  (*outlength) = new_length;
  chunk_start = &(*out)[new_length - total_chunk_length];

  for(i = 0; i < total_chunk_length; i++) chunk_start[i] = chunk[i];

  return 0;
}

unsigned lodepng_chunk_create(unsigned char** out, size_t* outlength, unsigned length,
                              const char* type, const unsigned char* data)
{
  unsigned i;
  unsigned char *chunk, *new_buffer;
  size_t new_length = (*outlength) + length + 12;
  if(new_length < length + 12 || new_length < (*outlength)) return 77; /*integer overflow happened*/
  new_buffer = (unsigned char*)lodepng_realloc(*out, new_length);
  if(!new_buffer) return 83; /*alloc fail*/
  (*out) = new_buffer;
  (*outlength) = new_length;
  chunk = &(*out)[(*outlength) - length - 12];

  /*1: length*/
  lodepng_set32bitInt(chunk, (unsigned)length);

  /*2: chunk name (4 letters)*/
  chunk[4] = type[0];
  chunk[5] = type[1];
  chunk[6] = type[2];
  chunk[7] = type[3];

  /*3: the data*/
  for(i = 0; i < length; i++) chunk[8 + i] = data[i];

  /*4: CRC (of the chunkname characters and the data)*/
  lodepng_chunk_generate_crc(chunk);

  return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* / Color types and such                                                   / */
/* ////////////////////////////////////////////////////////////////////////// */

/*return type is a LodePNG error code*/
static unsigned checkColorValidity(LodePNGColorType colortype, unsigned bd) /*bd = bitdepth*/
{
  switch(colortype)
  {
    case 0: if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16)) return 37; break; /*grey*/
    case 2: if(!(                                 bd == 8 || bd == 16)) return 37; break; /*RGB*/
    case 3: if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8            )) return 37; break; /*palette*/
    case 4: if(!(                                 bd == 8 || bd == 16)) return 37; break; /*grey + alpha*/
    case 6: if(!(                                 bd == 8 || bd == 16)) return 37; break; /*RGBA*/
    default: return 31;
  }
  return 0; /*allowed color type / bits combination*/
}

static unsigned getNumColorChannels(LodePNGColorType colortype)
{
  switch(colortype)
  {
    case 0: return 1; /*grey*/
    case 2: return 3; /*RGB*/
    case 3: return 1; /*palette*/
    case 4: return 2; /*grey + alpha*/
    case 6: return 4; /*RGBA*/
  }
  return 0; /*unexisting color type*/
}

static unsigned lodepng_get_bpp_lct(LodePNGColorType colortype, unsigned bitdepth)
{
  /*bits per pixel is amount of channels * bits per channel*/
  return getNumColorChannels(colortype) * bitdepth;
}

/* ////////////////////////////////////////////////////////////////////////// */

void lodepng_color_mode_init(LodePNGColorMode* info)
{
  info->key_defined = 0;
  info->key_r = info->key_g = info->key_b = 0;
  info->colortype = LCT_RGBA;
  info->bitdepth = 8;
  info->palette = 0;
  info->palettesize = 0;
}

void lodepng_color_mode_cleanup(LodePNGColorMode* info)
{
  lodepng_palette_clear(info);
}

unsigned lodepng_color_mode_copy(LodePNGColorMode* dest, const LodePNGColorMode* source)
{
  size_t i;
  lodepng_color_mode_cleanup(dest);
  *dest = *source;
  if(source->palette)
  {
    dest->palette = (unsigned char*)lodepng_malloc(1024);
    if(!dest->palette && source->palettesize) return 83; /*alloc fail*/
    for(i = 0; i < source->palettesize * 4; i++) dest->palette[i] = source->palette[i];
  }
  return 0;
}

static int lodepng_color_mode_equal(const LodePNGColorMode* a, const LodePNGColorMode* b)
{
  size_t i;
  if(a->colortype != b->colortype) return 0;
  if(a->bitdepth != b->bitdepth) return 0;
  if(a->key_defined != b->key_defined) return 0;
  if(a->key_defined)
  {
    if(a->key_r != b->key_r) return 0;
    if(a->key_g != b->key_g) return 0;
    if(a->key_b != b->key_b) return 0;
  }
  if(a->palettesize != b->palettesize) return 0;
  for(i = 0; i < a->palettesize * 4; i++)
  {
    if(a->palette[i] != b->palette[i]) return 0;
  }
  return 1;
}

void lodepng_palette_clear(LodePNGColorMode* info)
{
  if(info->palette) lodepng_free(info->palette);
  info->palette = 0;
  info->palettesize = 0;
}

unsigned lodepng_palette_add(LodePNGColorMode* info,
                             unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  unsigned char* data;
  /*the same resize technique as C++ std::vectors is used, and here it's made so that for a palette with
  the max of 256 colors, it'll have the exact alloc size*/
  if(!info->palette) /*allocate palette if empty*/
  {
    /*room for 256 colors with 4 bytes each*/
    data = (unsigned char*)lodepng_realloc(info->palette, 1024);
    if(!data) return 83; /*alloc fail*/
    else info->palette = data;
  }
  info->palette[4 * info->palettesize + 0] = r;
  info->palette[4 * info->palettesize + 1] = g;
  info->palette[4 * info->palettesize + 2] = b;
  info->palette[4 * info->palettesize + 3] = a;
  info->palettesize++;
  return 0;
}

unsigned lodepng_get_bpp(const LodePNGColorMode* info)
{
  /*calculate bits per pixel out of colortype and bitdepth*/
  return lodepng_get_bpp_lct(info->colortype, info->bitdepth);
}

unsigned lodepng_get_channels(const LodePNGColorMode* info)
{
  return getNumColorChannels(info->colortype);
}

unsigned lodepng_is_greyscale_type(const LodePNGColorMode* info)
{
  return info->colortype == LCT_GREY || info->colortype == LCT_GREY_ALPHA;
}

unsigned lodepng_is_alpha_type(const LodePNGColorMode* info)
{
  return (info->colortype & 4) != 0; /*4 or 6*/
}

unsigned lodepng_is_palette_type(const LodePNGColorMode* info)
{
  return info->colortype == LCT_PALETTE;
}

unsigned lodepng_has_palette_alpha(const LodePNGColorMode* info)
{
  size_t i;
  for(i = 0; i < info->palettesize; i++)
  {
    if(info->palette[i * 4 + 3] < 255) return 1;
  }
  return 0;
}

unsigned lodepng_can_have_alpha(const LodePNGColorMode* info)
{
  return info->key_defined
      || lodepng_is_alpha_type(info)
      || lodepng_has_palette_alpha(info);
}

size_t lodepng_get_raw_size(unsigned w, unsigned h, const LodePNGColorMode* color)
{
  return (w * h * lodepng_get_bpp(color) + 7) / 8;
}

size_t lodepng_get_raw_size_lct(unsigned w, unsigned h, LodePNGColorType colortype, unsigned bitdepth)
{
  return (w * h * lodepng_get_bpp_lct(colortype, bitdepth) + 7) / 8;
}

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

static void LodePNGUnknownChunks_init(LodePNGInfo* info)
{
  unsigned i;
  for(i = 0; i < 3; i++) info->unknown_chunks_data[i] = 0;
  for(i = 0; i < 3; i++) info->unknown_chunks_size[i] = 0;
}

static void LodePNGUnknownChunks_cleanup(LodePNGInfo* info)
{
  unsigned i;
  for(i = 0; i < 3; i++) lodepng_free(info->unknown_chunks_data[i]);
}

static unsigned LodePNGUnknownChunks_copy(LodePNGInfo* dest, const LodePNGInfo* src)
{
  unsigned i;

  LodePNGUnknownChunks_cleanup(dest);

  for(i = 0; i < 3; i++)
  {
    size_t j;
    dest->unknown_chunks_size[i] = src->unknown_chunks_size[i];
    dest->unknown_chunks_data[i] = (unsigned char*)lodepng_malloc(src->unknown_chunks_size[i]);
    if(!dest->unknown_chunks_data[i] && dest->unknown_chunks_size[i]) return 83; /*alloc fail*/
    for(j = 0; j < src->unknown_chunks_size[i]; j++)
    {
      dest->unknown_chunks_data[i][j] = src->unknown_chunks_data[i][j];
    }
  }

  return 0;
}

/******************************************************************************/

static void LodePNGText_init(LodePNGInfo* info)
{
  info->text_num = 0;
  info->text_keys = NULL;
  info->text_strings = NULL;
}

static void LodePNGText_cleanup(LodePNGInfo* info)
{
  size_t i;
  for(i = 0; i < info->text_num; i++)
  {
    string_cleanup(&info->text_keys[i]);
    string_cleanup(&info->text_strings[i]);
  }
  lodepng_free(info->text_keys);
  lodepng_free(info->text_strings);
}

static unsigned LodePNGText_copy(LodePNGInfo* dest, const LodePNGInfo* source)
{
  size_t i = 0;
  dest->text_keys = 0;
  dest->text_strings = 0;
  dest->text_num = 0;
  for(i = 0; i < source->text_num; i++)
  {
    CERROR_TRY_RETURN(lodepng_add_text(dest, source->text_keys[i], source->text_strings[i]));
  }
  return 0;
}

void lodepng_clear_text(LodePNGInfo* info)
{
  LodePNGText_cleanup(info);
}

unsigned lodepng_add_text(LodePNGInfo* info, const char* key, const char* str)
{
  char** new_keys = (char**)(lodepng_realloc(info->text_keys, sizeof(char*) * (info->text_num + 1)));
  char** new_strings = (char**)(lodepng_realloc(info->text_strings, sizeof(char*) * (info->text_num + 1)));
  if(!new_keys || !new_strings)
  {
    lodepng_free(new_keys);
    lodepng_free(new_strings);
    return 83; /*alloc fail*/
  }

  info->text_num++;
  info->text_keys = new_keys;
  info->text_strings = new_strings;

  string_init(&info->text_keys[info->text_num - 1]);
  string_set(&info->text_keys[info->text_num - 1], key);

  string_init(&info->text_strings[info->text_num - 1]);
  string_set(&info->text_strings[info->text_num - 1], str);

  return 0;
}

/******************************************************************************/

static void LodePNGIText_init(LodePNGInfo* info)
{
  info->itext_num = 0;
  info->itext_keys = NULL;
  info->itext_langtags = NULL;
  info->itext_transkeys = NULL;
  info->itext_strings = NULL;
}

static void LodePNGIText_cleanup(LodePNGInfo* info)
{
  size_t i;
  for(i = 0; i < info->itext_num; i++)
  {
    string_cleanup(&info->itext_keys[i]);
    string_cleanup(&info->itext_langtags[i]);
    string_cleanup(&info->itext_transkeys[i]);
    string_cleanup(&info->itext_strings[i]);
  }
  lodepng_free(info->itext_keys);
  lodepng_free(info->itext_langtags);
  lodepng_free(info->itext_transkeys);
  lodepng_free(info->itext_strings);
}

static unsigned LodePNGIText_copy(LodePNGInfo* dest, const LodePNGInfo* source)
{
  size_t i = 0;
  dest->itext_keys = 0;
  dest->itext_langtags = 0;
  dest->itext_transkeys = 0;
  dest->itext_strings = 0;
  dest->itext_num = 0;
  for(i = 0; i < source->itext_num; i++)
  {
    CERROR_TRY_RETURN(lodepng_add_itext(dest, source->itext_keys[i], source->itext_langtags[i],
                                        source->itext_transkeys[i], source->itext_strings[i]));
  }
  return 0;
}

void lodepng_clear_itext(LodePNGInfo* info)
{
  LodePNGIText_cleanup(info);
}

unsigned lodepng_add_itext(LodePNGInfo* info, const char* key, const char* langtag,
                           const char* transkey, const char* str)
{
  char** new_keys = (char**)(lodepng_realloc(info->itext_keys, sizeof(char*) * (info->itext_num + 1)));
  char** new_langtags = (char**)(lodepng_realloc(info->itext_langtags, sizeof(char*) * (info->itext_num + 1)));
  char** new_transkeys = (char**)(lodepng_realloc(info->itext_transkeys, sizeof(char*) * (info->itext_num + 1)));
  char** new_strings = (char**)(lodepng_realloc(info->itext_strings, sizeof(char*) * (info->itext_num + 1)));
  if(!new_keys || !new_langtags || !new_transkeys || !new_strings)
  {
    lodepng_free(new_keys);
    lodepng_free(new_langtags);
    lodepng_free(new_transkeys);
    lodepng_free(new_strings);
    return 83; /*alloc fail*/
  }

  info->itext_num++;
  info->itext_keys = new_keys;
  info->itext_langtags = new_langtags;
  info->itext_transkeys = new_transkeys;
  info->itext_strings = new_strings;

  string_init(&info->itext_keys[info->itext_num - 1]);
  string_set(&info->itext_keys[info->itext_num - 1], key);

  string_init(&info->itext_langtags[info->itext_num - 1]);
  string_set(&info->itext_langtags[info->itext_num - 1], langtag);

  string_init(&info->itext_transkeys[info->itext_num - 1]);
  string_set(&info->itext_transkeys[info->itext_num - 1], transkey);

  string_init(&info->itext_strings[info->itext_num - 1]);
  string_set(&info->itext_strings[info->itext_num - 1], str);

  return 0;
}
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

void lodepng_info_init(LodePNGInfo* info)
{
  lodepng_color_mode_init(&info->color);
  info->interlace_method = 0;
  info->compression_method = 0;
  info->filter_method = 0;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  info->background_defined = 0;
  info->background_r = info->background_g = info->background_b = 0;

  LodePNGText_init(info);
  LodePNGIText_init(info);

  info->time_defined = 0;
  info->phys_defined = 0;

  LodePNGUnknownChunks_init(info);
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
}

void lodepng_info_cleanup(LodePNGInfo* info)
{
  lodepng_color_mode_cleanup(&info->color);
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  LodePNGText_cleanup(info);
  LodePNGIText_cleanup(info);

  LodePNGUnknownChunks_cleanup(info);
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
}

unsigned lodepng_info_copy(LodePNGInfo* dest, const LodePNGInfo* source)
{
  lodepng_info_cleanup(dest);
  *dest = *source;
  lodepng_color_mode_init(&dest->color);
  CERROR_TRY_RETURN(lodepng_color_mode_copy(&dest->color, &source->color));

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  CERROR_TRY_RETURN(LodePNGText_copy(dest, source));
  CERROR_TRY_RETURN(LodePNGIText_copy(dest, source));

  LodePNGUnknownChunks_init(dest);
  CERROR_TRY_RETURN(LodePNGUnknownChunks_copy(dest, source));
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
  return 0;
}

void lodepng_info_swap(LodePNGInfo* a, LodePNGInfo* b)
{
  LodePNGInfo temp = *a;
  *a = *b;
  *b = temp;
}

/* ////////////////////////////////////////////////////////////////////////// */

/*index: bitgroup index, bits: bitgroup size(1, 2 or 4, in: bitgroup value, out: octet array to add bits to*/
static void addColorBits(unsigned char* out, size_t index, unsigned bits, unsigned in)
{
  /*p = the partial index in the byte, e.g. with 4 palettebits it is 0 for first half or 1 for second half*/
  unsigned p = index % (8 / bits);
  in &= (1 << bits) - 1; /*filter out any other bits of the input value*/
  in = in << (bits * (8 / bits - p - 1));
  if(p == 0) out[index * bits / 8] = in;
  else out[index * bits / 8] |= in;
}

typedef struct ColorTree ColorTree;

/*
One node of a color tree
This is the data structure used to count the number of unique colors and to get a palette
index for a color. It's like an octree, but because the alpha channel is used too, each
node has 16 instead of 8 children.
*/
struct ColorTree
{
  ColorTree* children[16]; /*up to 16 pointers to ColorTree of next level*/
  int index; /*the payload. Only has a meaningful value if this is in the last level*/
};

static void color_tree_init(ColorTree* tree)
{
  int i;
  for(i = 0; i < 16; i++) tree->children[i] = 0;
  tree->index = -1;
}

static void color_tree_cleanup(ColorTree* tree)
{
  int i;
  for(i = 0; i < 16; i++)
  {
    if(tree->children[i])
    {
      color_tree_cleanup(tree->children[i]);
      lodepng_free(tree->children[i]);
    }
  }
}

/*returns -1 if color not present, its index otherwise*/
static int color_tree_get(ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  int bit = 0;
  for(bit = 0; bit < 8; bit++)
  {
    int i = 8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1) + 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1);
    if(!tree->children[i]) return -1;
    else tree = tree->children[i];
  }
  return tree ? tree->index : -1;
}

#ifdef LODEPNG_COMPILE_ENCODER
static int color_tree_has(ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  return color_tree_get(tree, r, g, b, a) >= 0;
}
#endif /*LODEPNG_COMPILE_ENCODER*/

/*color is not allowed to already exist.
Index should be >= 0 (it's signed to be compatible with using -1 for "doesn't exist")*/
static void color_tree_add(ColorTree* tree,
                           unsigned char r, unsigned char g, unsigned char b, unsigned char a, int index)
{
  int bit;
  for(bit = 0; bit < 8; bit++)
  {
    int i = 8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1) + 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1);
    if(!tree->children[i])
    {
      tree->children[i] = (ColorTree*)lodepng_malloc(sizeof(ColorTree));
      color_tree_init(tree->children[i]);
    }
    tree = tree->children[i];
  }
  tree->index = index;
}

/*put a pixel, given its RGBA color, into image of any color type*/
static unsigned rgba8ToPixel(unsigned char* out, size_t i,
                             const LodePNGColorMode* mode, ColorTree* tree /*for palette*/,
                             unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  if(mode->colortype == LCT_GREY)
  {
    unsigned char grey = r; /*((unsigned short)r + g + b) / 3*/;
    if(mode->bitdepth == 8) out[i] = grey;
    else if(mode->bitdepth == 16) out[i * 2 + 0] = out[i * 2 + 1] = grey;
    else
    {
      /*take the most significant bits of grey*/
      grey = (grey >> (8 - mode->bitdepth)) & ((1 << mode->bitdepth) - 1);
      addColorBits(out, i, mode->bitdepth, grey);
    }
  }
  else if(mode->colortype == LCT_RGB)
  {
    if(mode->bitdepth == 8)
    {
      out[i * 3 + 0] = r;
      out[i * 3 + 1] = g;
      out[i * 3 + 2] = b;
    }
    else
    {
      out[i * 6 + 0] = out[i * 6 + 1] = r;
      out[i * 6 + 2] = out[i * 6 + 3] = g;
      out[i * 6 + 4] = out[i * 6 + 5] = b;
    }
  }
  else if(mode->colortype == LCT_PALETTE)
  {
    int index = color_tree_get(tree, r, g, b, a);
    if(index < 0) return 82; /*color not in palette*/
    if(mode->bitdepth == 8) out[i] = index;
    else addColorBits(out, i, mode->bitdepth, index);
  }
  else if(mode->colortype == LCT_GREY_ALPHA)
  {
    unsigned char grey = r; /*((unsigned short)r + g + b) / 3*/;
    if(mode->bitdepth == 8)
    {
      out[i * 2 + 0] = grey;
      out[i * 2 + 1] = a;
    }
    else if(mode->bitdepth == 16)
    {
      out[i * 4 + 0] = out[i * 4 + 1] = grey;
      out[i * 4 + 2] = out[i * 4 + 3] = a;
    }
  }
  else if(mode->colortype == LCT_RGBA)
  {
    if(mode->bitdepth == 8)
    {
      out[i * 4 + 0] = r;
      out[i * 4 + 1] = g;
      out[i * 4 + 2] = b;
      out[i * 4 + 3] = a;
    }
    else
    {
      out[i * 8 + 0] = out[i * 8 + 1] = r;
      out[i * 8 + 2] = out[i * 8 + 3] = g;
      out[i * 8 + 4] = out[i * 8 + 5] = b;
      out[i * 8 + 6] = out[i * 8 + 7] = a;
    }
  }

  return 0; /*no error*/
}

/*put a pixel, given its RGBA16 color, into image of any color 16-bitdepth type*/
static unsigned rgba16ToPixel(unsigned char* out, size_t i,
                              const LodePNGColorMode* mode,
                              unsigned short r, unsigned short g, unsigned short b, unsigned short a)
{
  if(mode->bitdepth != 16) return 85; /*must be 16 for this function*/
  if(mode->colortype == LCT_GREY)
  {
    unsigned short grey = r; /*((unsigned)r + g + b) / 3*/;
    out[i * 2 + 0] = (grey >> 8) & 255;
    out[i * 2 + 1] = grey & 255;
  }
  else if(mode->colortype == LCT_RGB)
  {
    out[i * 6 + 0] = (r >> 8) & 255;
    out[i * 6 + 1] = r & 255;
    out[i * 6 + 2] = (g >> 8) & 255;
    out[i * 6 + 3] = g & 255;
    out[i * 6 + 4] = (b >> 8) & 255;
    out[i * 6 + 5] = b & 255;
  }
  else if(mode->colortype == LCT_GREY_ALPHA)
  {
    unsigned short grey = r; /*((unsigned)r + g + b) / 3*/;
    out[i * 4 + 0] = (grey >> 8) & 255;
    out[i * 4 + 1] = grey & 255;
    out[i * 4 + 2] = (a >> 8) & 255;
    out[i * 4 + 3] = a & 255;
  }
  else if(mode->colortype == LCT_RGBA)
  {
    out[i * 8 + 0] = (r >> 8) & 255;
    out[i * 8 + 1] = r & 255;
    out[i * 8 + 2] = (g >> 8) & 255;
    out[i * 8 + 3] = g & 255;
    out[i * 8 + 4] = (b >> 8) & 255;
    out[i * 8 + 5] = b & 255;
    out[i * 8 + 6] = (a >> 8) & 255;
    out[i * 8 + 7] = a & 255;
  }

  return 0; /*no error*/
}

/*Get RGBA8 color of pixel with index i (y * width + x) from the raw image with given color type.*/
static unsigned getPixelColorRGBA8(unsigned char* r, unsigned char* g,
                                   unsigned char* b, unsigned char* a,
                                   const unsigned char* in, size_t i,
                                   const LodePNGColorMode* mode,
                                   unsigned fix_png)
{
  if(mode->colortype == LCT_GREY)
  {
    if(mode->bitdepth == 8)
    {
      *r = *g = *b = in[i];
      if(mode->key_defined && *r == mode->key_r) *a = 0;
      else *a = 255;
    }
    else if(mode->bitdepth == 16)
    {
      *r = *g = *b = in[i * 2 + 0];
      if(mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r) *a = 0;
      else *a = 255;
    }
    else
    {
      unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
      size_t j = i * mode->bitdepth;
      unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
      *r = *g = *b = (value * 255) / highest;
      if(mode->key_defined && value == mode->key_r) *a = 0;
      else *a = 255;
    }
  }
  else if(mode->colortype == LCT_RGB)
  {
    if(mode->bitdepth == 8)
    {
      *r = in[i * 3 + 0]; *g = in[i * 3 + 1]; *b = in[i * 3 + 2];
      if(mode->key_defined && *r == mode->key_r && *g == mode->key_g && *b == mode->key_b) *a = 0;
      else *a = 255;
    }
    else
    {
      *r = in[i * 6 + 0];
      *g = in[i * 6 + 2];
      *b = in[i * 6 + 4];
      if(mode->key_defined && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
         && 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
         && 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b) *a = 0;
      else *a = 255;
    }
  }
  else if(mode->colortype == LCT_PALETTE)
  {
    unsigned index;
    if(mode->bitdepth == 8) index = in[i];
    else
    {
      size_t j = i * mode->bitdepth;
      index = readBitsFromReversedStream(&j, in, mode->bitdepth);
    }

    if(index >= mode->palettesize)
    {
      /*This is an error according to the PNG spec, but fix_png can ignore it*/
      if(!fix_png) return (mode->bitdepth == 8 ? 46 : 47); /*index out of palette*/
      *r = *g = *b = 0;
      *a = 255;
    }
    else
    {
      *r = mode->palette[index * 4 + 0];
      *g = mode->palette[index * 4 + 1];
      *b = mode->palette[index * 4 + 2];
      *a = mode->palette[index * 4 + 3];
    }
  }
  else if(mode->colortype == LCT_GREY_ALPHA)
  {
    if(mode->bitdepth == 8)
    {
      *r = *g = *b = in[i * 2 + 0];
      *a = in[i * 2 + 1];
    }
    else
    {
      *r = *g = *b = in[i * 4 + 0];
      *a = in[i * 4 + 2];
    }
  }
  else if(mode->colortype == LCT_RGBA)
  {
    if(mode->bitdepth == 8)
    {
      *r = in[i * 4 + 0];
      *g = in[i * 4 + 1];
      *b = in[i * 4 + 2];
      *a = in[i * 4 + 3];
    }
    else
    {
      *r = in[i * 8 + 0];
      *g = in[i * 8 + 2];
      *b = in[i * 8 + 4];
      *a = in[i * 8 + 6];
    }
  }

  return 0; /*no error*/
}

/*Similar to getPixelColorRGBA8, but with all the for loops inside of the color
mode test cases, optimized to convert the colors much faster, when converting
to RGBA or RGB with 8 bit per cannel. buffer must be RGBA or RGB output with
enough memory, if has_alpha is true the output is RGBA. mode has the color mode
of the input buffer.*/
static unsigned getPixelColorsRGBA8(unsigned char* buffer, size_t numpixels,
                                    unsigned has_alpha, const unsigned char* in,
                                    const LodePNGColorMode* mode,
                                    unsigned fix_png)
{
  unsigned num_channels = has_alpha ? 4 : 3;
  size_t i;
  if(mode->colortype == LCT_GREY)
  {
    if(mode->bitdepth == 8)
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = buffer[1] = buffer[2] = in[i];
        if(has_alpha) buffer[3] = mode->key_defined && in[i] == mode->key_r ? 0 : 255;
      }
    }
    else if(mode->bitdepth == 16)
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = buffer[1] = buffer[2] = in[i * 2];
        if(has_alpha) buffer[3] = mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r ? 0 : 255;
      }
    }
    else
    {
      unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
      size_t j = 0;
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
        buffer[0] = buffer[1] = buffer[2] = (value * 255) / highest;
        if(has_alpha) buffer[3] = mode->key_defined && value == mode->key_r ? 0 : 255;
      }
    }
  }
  else if(mode->colortype == LCT_RGB)
  {
    if(mode->bitdepth == 8)
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = in[i * 3 + 0];
        buffer[1] = in[i * 3 + 1];
        buffer[2] = in[i * 3 + 2];
        if(has_alpha) buffer[3] = mode->key_defined && buffer[0] == mode->key_r
           && buffer[1]== mode->key_g && buffer[2] == mode->key_b ? 0 : 255;
      }
    }
    else
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = in[i * 6 + 0];
        buffer[1] = in[i * 6 + 2];
        buffer[2] = in[i * 6 + 4];
        if(has_alpha) buffer[3] = mode->key_defined
           && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
           && 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
           && 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b ? 0 : 255;
      }
    }
  }
  else if(mode->colortype == LCT_PALETTE)
  {
    unsigned index;
    size_t j = 0;
    for(i = 0; i < numpixels; i++, buffer += num_channels)
    {
      if(mode->bitdepth == 8) index = in[i];
      else index = readBitsFromReversedStream(&j, in, mode->bitdepth);

      if(index >= mode->palettesize)
      {
        /*This is an error according to the PNG spec, but fix_png can ignore it*/
        if(!fix_png) return (mode->bitdepth == 8 ? 46 : 47); /*index out of palette*/
        buffer[0] = buffer[1] = buffer[2] = 0;
        if(has_alpha) buffer[3] = 255;
      }
      else
      {
        buffer[0] = mode->palette[index * 4 + 0];
        buffer[1] = mode->palette[index * 4 + 1];
        buffer[2] = mode->palette[index * 4 + 2];
        if(has_alpha) buffer[3] = mode->palette[index * 4 + 3];
      }
    }
  }
  else if(mode->colortype == LCT_GREY_ALPHA)
  {
    if(mode->bitdepth == 8)
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = buffer[1] = buffer[2] = in[i * 2 + 0];
        if(has_alpha) buffer[3] = in[i * 2 + 1];
      }
    }
    else
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = buffer[1] = buffer[2] = in[i * 4 + 0];
        if(has_alpha) buffer[3] = in[i * 4 + 2];
      }
    }
  }
  else if(mode->colortype == LCT_RGBA)
  {
    if(mode->bitdepth == 8)
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = in[i * 4 + 0];
        buffer[1] = in[i * 4 + 1];
        buffer[2] = in[i * 4 + 2];
        if(has_alpha) buffer[3] = in[i * 4 + 3];
      }
    }
    else
    {
      for(i = 0; i < numpixels; i++, buffer += num_channels)
      {
        buffer[0] = in[i * 8 + 0];
        buffer[1] = in[i * 8 + 2];
        buffer[2] = in[i * 8 + 4];
        if(has_alpha) buffer[3] = in[i * 8 + 6];
      }
    }
  }

  return 0; /*no error*/
}

/*Get RGBA16 color of pixel with index i (y * width + x) from the raw image with
given color type, but the given color type must be 16-bit itself.*/
static unsigned getPixelColorRGBA16(unsigned short* r, unsigned short* g, unsigned short* b, unsigned short* a,
                                    const unsigned char* in, size_t i, const LodePNGColorMode* mode)
{
  if(mode->bitdepth != 16) return 85; /*error: this function only supports 16-bit input*/

  if(mode->colortype == LCT_GREY)
  {
    *r = *g = *b = 256 * in[i * 2 + 0] + in[i * 2 + 1];
    if(mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r) *a = 0;
    else *a = 65535;
  }
  else if(mode->colortype == LCT_RGB)
  {
    *r = 256 * in[i * 6 + 0] + in[i * 6 + 1];
    *g = 256 * in[i * 6 + 2] + in[i * 6 + 3];
    *b = 256 * in[i * 6 + 4] + in[i * 6 + 5];
    if(mode->key_defined && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
       && 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
       && 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b) *a = 0;
    else *a = 65535;
  }
  else if(mode->colortype == LCT_GREY_ALPHA)
  {
    *r = *g = *b = 256 * in[i * 4 + 0] + in[i * 4 + 1];
    *a = 256 * in[i * 4 + 2] + in[i * 4 + 3];
  }
  else if(mode->colortype == LCT_RGBA)
  {
    *r = 256 * in[i * 8 + 0] + in[i * 8 + 1];
    *g = 256 * in[i * 8 + 2] + in[i * 8 + 3];
    *b = 256 * in[i * 8 + 4] + in[i * 8 + 5];
    *a = 256 * in[i * 8 + 6] + in[i * 8 + 7];
  }
  else return 85; /*error: this function only supports 16-bit input, not palettes*/

  return 0; /*no error*/
}

/*
converts from any color type to 24-bit or 32-bit (later maybe more supported). return value = LodePNG error code
the out buffer must have (w * h * bpp + 7) / 8 bytes, where bpp is the bits per pixel of the output color type
(lodepng_get_bpp) for < 8 bpp images, there may _not_ be padding bits at the end of scanlines.
*/
unsigned lodepng_convert(unsigned char* out, const unsigned char* in,
                         LodePNGColorMode* mode_out, LodePNGColorMode* mode_in,
                         unsigned w, unsigned h, unsigned fix_png)
{
  unsigned error = 0;
  size_t i;
  ColorTree tree;
  size_t numpixels = w * h;

  if(lodepng_color_mode_equal(mode_out, mode_in))
  {
    size_t numbytes = lodepng_get_raw_size(w, h, mode_in);
    for(i = 0; i < numbytes; i++) out[i] = in[i];
    return error;
  }

  if(mode_out->colortype == LCT_PALETTE)
  {
    size_t palsize = 1 << mode_out->bitdepth;
    if(mode_out->palettesize < palsize) palsize = mode_out->palettesize;
    color_tree_init(&tree);
    for(i = 0; i < palsize; i++)
    {
      unsigned char* p = &mode_out->palette[i * 4];
      color_tree_add(&tree, p[0], p[1], p[2], p[3], i);
    }
  }

  if(mode_in->bitdepth == 16 && mode_out->bitdepth == 16)
  {
    for(i = 0; i < numpixels; i++)
    {
      unsigned short r = 0, g = 0, b = 0, a = 0;
      error = getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode_in);
      if(error) break;
      error = rgba16ToPixel(out, i, mode_out, r, g, b, a);
      if(error) break;
    }
  }
  else if(mode_out->bitdepth == 8 && mode_out->colortype == LCT_RGBA)
  {
    error = getPixelColorsRGBA8(out, numpixels, 1, in, mode_in, fix_png);
  }
  else if(mode_out->bitdepth == 8 && mode_out->colortype == LCT_RGB)
  {
    error = getPixelColorsRGBA8(out, numpixels, 0, in, mode_in, fix_png);
  }
  else
  {
    unsigned char r = 0, g = 0, b = 0, a = 0;
    for(i = 0; i < numpixels; i++)
    {
      error = getPixelColorRGBA8(&r, &g, &b, &a, in, i, mode_in, fix_png);
      if(error) break;
      error = rgba8ToPixel(out, i, mode_out, &tree, r, g, b, a);
      if(error) break;
    }
  }

  if(mode_out->colortype == LCT_PALETTE)
  {
    color_tree_cleanup(&tree);
  }

  return error;
}

#ifdef LODEPNG_COMPILE_ENCODER

typedef struct ColorProfile
{
  unsigned char sixteenbit; /*needs more than 8 bits per channel*/
  unsigned char sixteenbit_done;


  unsigned char colored; /*not greyscale*/
  unsigned char colored_done;

  unsigned char key; /*a color key is required, or more*/
  unsigned short key_r; /*these values are always in 16-bit bitdepth in the profile*/
  unsigned short key_g;
  unsigned short key_b;
  unsigned char alpha; /*alpha channel, or alpha palette, required*/
  unsigned char alpha_done;

  unsigned numcolors;
  ColorTree tree; /*for listing the counted colors, up to 256*/
  unsigned char* palette; /*size 1024. Remember up to the first 256 RGBA colors*/
  unsigned maxnumcolors; /*if more than that amount counted*/
  unsigned char numcolors_done;

  unsigned greybits; /*amount of bits required for greyscale (1, 2, 4, 8). Does not take 16 bit into account.*/
  unsigned char greybits_done;

} ColorProfile;

static void color_profile_init(ColorProfile* profile, LodePNGColorMode* mode)
{
  profile->sixteenbit = 0;
  profile->sixteenbit_done = mode->bitdepth == 16 ? 0 : 1;

  profile->colored = 0;
  profile->colored_done = lodepng_is_greyscale_type(mode) ? 1 : 0;

  profile->key = 0;
  profile->alpha = 0;
  profile->alpha_done = lodepng_can_have_alpha(mode) ? 0 : 1;

  profile->numcolors = 0;
  color_tree_init(&profile->tree);
  profile->palette = (unsigned char*)lodepng_malloc(1024);
  profile->maxnumcolors = 257;
  if(lodepng_get_bpp(mode) <= 8)
  {
    int bpp = lodepng_get_bpp(mode);
    profile->maxnumcolors = bpp == 1 ? 2 : (bpp == 2 ? 4 : (bpp == 4 ? 16 : 256));
  }
  profile->numcolors_done = 0;

  profile->greybits = 1;
  profile->greybits_done = lodepng_get_bpp(mode) == 1 ? 1 : 0;
}

static void color_profile_cleanup(ColorProfile* profile)
{
  color_tree_cleanup(&profile->tree);
  lodepng_free(profile->palette);
}

/*function used for debug purposes with C++*/
/*void printColorProfile(ColorProfile* p)
{
  std::cout << "sixteenbit: " << (int)p->sixteenbit << std::endl;
  std::cout << "sixteenbit_done: " << (int)p->sixteenbit_done << std::endl;
  std::cout << "colored: " << (int)p->colored << std::endl;
  std::cout << "colored_done: " << (int)p->colored_done << std::endl;
  std::cout << "key: " << (int)p->key << std::endl;
  std::cout << "key_r: " << (int)p->key_r << std::endl;
  std::cout << "key_g: " << (int)p->key_g << std::endl;
  std::cout << "key_b: " << (int)p->key_b << std::endl;
  std::cout << "alpha: " << (int)p->alpha << std::endl;
  std::cout << "alpha_done: " << (int)p->alpha_done << std::endl;
  std::cout << "numcolors: " << (int)p->numcolors << std::endl;
  std::cout << "maxnumcolors: " << (int)p->maxnumcolors << std::endl;
  std::cout << "numcolors_done: " << (int)p->numcolors_done << std::endl;
  std::cout << "greybits: " << (int)p->greybits << std::endl;
  std::cout << "greybits_done: " << (int)p->greybits_done << std::endl;
}*/

/*Returns how many bits needed to represent given value (max 8 bit)*/
unsigned getValueRequiredBits(unsigned short value)
{
  if(value == 0 || value == 255) return 1;
  /*The scaling of 2-bit and 4-bit values uses multiples of 85 and 17*/
  if(value % 17 == 0) return value % 85 == 0 ? 2 : 4;
  return 8;
}

/*profile must already have been inited with mode.
It's ok to set some parameters of profile to done already.*/
static unsigned get_color_profile(ColorProfile* profile,
                                  const unsigned char* in,
                                  size_t numpixels /*must be full image size, for certain filesize based choices*/,
                                  LodePNGColorMode* mode,
                                  unsigned fix_png)
{
  unsigned error = 0;
  size_t i;

  if(mode->bitdepth == 16)
  {
    for(i = 0; i < numpixels; i++)
    {
      unsigned short r, g, b, a;
      error = getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode);
      if(error) break;

      /*a color is considered good for 8-bit if the first byte and the second byte are equal,
        (so if it's divisible through 257), NOT necessarily if the second byte is 0*/
      if(!profile->sixteenbit_done
          && (((r & 255) != ((r >> 8) & 255))
           || ((g & 255) != ((g >> 8) & 255))
           || ((b & 255) != ((b >> 8) & 255))))
      {
        profile->sixteenbit = 1;
        profile->sixteenbit_done = 1;
        profile->greybits_done = 1; /*greybits is not applicable anymore at 16-bit*/
        profile->numcolors_done = 1; /*counting colors no longer useful, palette doesn't support 16-bit*/
      }

      if(!profile->colored_done && (r != g || r != b))
      {
        profile->colored = 1;
        profile->colored_done = 1;
        profile->greybits_done = 1; /*greybits is not applicable anymore*/
      }

      if(!profile->alpha_done && a != 65535)
      {
        /*only use color key if numpixels large enough to justify tRNS chunk size*/
        if(a == 0 && numpixels > 16 && !(profile->key && (r != profile->key_r || g != profile->key_g || b != profile->key_b)))
        {
          if(!profile->alpha && !profile->key)
          {
            profile->key = 1;
            profile->key_r = r;
            profile->key_g = g;
            profile->key_b = b;
          }
        }
        else
        {
          profile->alpha = 1;
          profile->alpha_done = 1;
          profile->greybits_done = 1; /*greybits is not applicable anymore*/
        }
      }

      /* Color key cannot be used if an opaque pixel also has that RGB color. */
      if(!profile->alpha_done && a == 65535 && profile->key
          && r == profile->key_r && g == profile->key_g && b == profile->key_b)
      {
          profile->alpha = 1;
          profile->alpha_done = 1;
          profile->greybits_done = 1; /*greybits is not applicable anymore*/
      }

      if(!profile->greybits_done)
      {
        /*assuming 8-bit r, this test does not care about 16-bit*/
        unsigned bits = getValueRequiredBits(r);
        if(bits > profile->greybits) profile->greybits = bits;
        if(profile->greybits >= 8) profile->greybits_done = 1;
      }

      if(!profile->numcolors_done)
      {
        /*assuming 8-bit rgba, this test does not care about 16-bit*/
        if(!color_tree_has(&profile->tree, (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a))
        {
          color_tree_add(&profile->tree, (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a,
            profile->numcolors);
          if(profile->numcolors < 256)
          {
            unsigned char* p = profile->palette;
            unsigned i = profile->numcolors;
            p[i * 4 + 0] = (unsigned char)r;
            p[i * 4 + 1] = (unsigned char)g;
            p[i * 4 + 2] = (unsigned char)b;
            p[i * 4 + 3] = (unsigned char)a;
          }
          profile->numcolors++;
          if(profile->numcolors >= profile->maxnumcolors) profile->numcolors_done = 1;
        }
      }

      if(profile->alpha_done && profile->numcolors_done
      && profile->colored_done && profile->sixteenbit_done && profile->greybits_done)
      {
        break;
      }
    };
  }
  else /* < 16-bit */
  {
    for(i = 0; i < numpixels; i++)
    {
      unsigned char r = 0, g = 0, b = 0, a = 0;
      error = getPixelColorRGBA8(&r, &g, &b, &a, in, i, mode, fix_png);
      if(error) break;

      if(!profile->colored_done && (r != g || r != b))
      {
        profile->colored = 1;
        profile->colored_done = 1;
        profile->greybits_done = 1; /*greybits is not applicable anymore*/
      }

      if(!profile->alpha_done && a != 255)
      {
        if(a == 0 && !(profile->key && (r != profile->key_r || g != profile->key_g || b != profile->key_b)))
        {
          if(!profile->key)
          {
            profile->key = 1;
            profile->key_r = r;
            profile->key_g = g;
            profile->key_b = b;
          }
        }
        else
        {
          profile->alpha = 1;
          profile->alpha_done = 1;
          profile->greybits_done = 1; /*greybits is not applicable anymore*/
        }
      }

      /* Color key cannot be used if an opaque pixel also has that RGB color. */
      if(!profile->alpha_done && a == 255 && profile->key
          && r == profile->key_r && g == profile->key_g && b == profile->key_b)
      {
          profile->alpha = 1;
          profile->alpha_done = 1;
          profile->greybits_done = 1; /*greybits is not applicable anymore*/
      }

      if(!profile->greybits_done)
      {
        unsigned bits = getValueRequiredBits(r);
        if(bits > profile->greybits) profile->greybits = bits;
        if(profile->greybits >= 8) profile->greybits_done = 1;
      }

      if(!profile->numcolors_done)
      {
        if(!color_tree_has(&profile->tree, r, g, b, a))
        {

          color_tree_add(&profile->tree, r, g, b, a, profile->numcolors);
          if(profile->numcolors < 256)
          {
            unsigned char* p = profile->palette;
            unsigned i = profile->numcolors;
            p[i * 4 + 0] = r;
            p[i * 4 + 1] = g;
            p[i * 4 + 2] = b;
            p[i * 4 + 3] = a;
          }
          profile->numcolors++;
          if(profile->numcolors >= profile->maxnumcolors) profile->numcolors_done = 1;
        }
      }

      if(profile->alpha_done && profile->numcolors_done && profile->colored_done && profile->greybits_done)
      {
        break;
      }
    };
  }

  /*make the profile's key always 16-bit for consistency*/
  if(mode->bitdepth < 16)
  {
    /*repeat each byte twice*/
    profile->key_r *= 257;
    profile->key_g *= 257;
    profile->key_b *= 257;
  }

  return error;
}

static void setColorKeyFrom16bit(LodePNGColorMode* mode_out, unsigned r, unsigned g, unsigned b, unsigned bitdepth)
{
  unsigned mask = (1 << bitdepth) - 1;
  mode_out->key_defined = 1;
  mode_out->key_r = r & mask;
  mode_out->key_g = g & mask;
  mode_out->key_b = b & mask;
}

/*updates values of mode with a potentially smaller color model. mode_out should
contain the user chosen color model, but will be overwritten with the new chosen one.*/
static unsigned doAutoChooseColor(LodePNGColorMode* mode_out,
                                  const unsigned char* image, unsigned w, unsigned h, LodePNGColorMode* mode_in,
                                  LodePNGAutoConvert auto_convert)
{
  ColorProfile profile;
  unsigned error = 0;
  int no_nibbles = auto_convert == LAC_AUTO_NO_NIBBLES || auto_convert == LAC_AUTO_NO_NIBBLES_NO_PALETTE;
  int no_palette = auto_convert == LAC_AUTO_NO_PALETTE || auto_convert == LAC_AUTO_NO_NIBBLES_NO_PALETTE;

  if(auto_convert == LAC_ALPHA)
  {
    if(mode_out->colortype != LCT_RGBA && mode_out->colortype != LCT_GREY_ALPHA) return 0;
  }

  color_profile_init(&profile, mode_in);
  if(auto_convert == LAC_ALPHA)
  {
    profile.colored_done = 1;
    profile.greybits_done = 1;
    profile.numcolors_done = 1;
    profile.sixteenbit_done = 1;
  }
  error = get_color_profile(&profile, image, w * h, mode_in, 0 /*fix_png*/);
  if(!error && auto_convert == LAC_ALPHA)
  {
    if(!profile.alpha)
    {
      mode_out->colortype = (mode_out->colortype == LCT_RGBA ? LCT_RGB : LCT_GREY);
      if(profile.key) setColorKeyFrom16bit(mode_out, profile.key_r, profile.key_g, profile.key_b, mode_out->bitdepth);
    }
  }
  else if(!error && auto_convert != LAC_ALPHA)
  {
    mode_out->key_defined = 0;

    if(profile.sixteenbit)
    {
      mode_out->bitdepth = 16;
      if(profile.alpha)
      {
        mode_out->colortype = profile.colored ? LCT_RGBA : LCT_GREY_ALPHA;
      }
      else
      {
        mode_out->colortype = profile.colored ? LCT_RGB : LCT_GREY;
        if(profile.key) setColorKeyFrom16bit(mode_out, profile.key_r, profile.key_g, profile.key_b, mode_out->bitdepth);
      }
    }
    else /*less than 16 bits per channel*/
    {
      /*don't add palette overhead if image hasn't got a lot of pixels*/
      unsigned n = profile.numcolors;
      int palette_ok = !no_palette && n <= 256 && (n * 2 < w * h);
      unsigned palettebits = n <= 2 ? 1 : (n <= 4 ? 2 : (n <= 16 ? 4 : 8));
      int grey_ok = !profile.colored && !profile.alpha; /*grey without alpha, with potentially low bits*/
      if(palette_ok || grey_ok)
      {
        if(!palette_ok || (grey_ok && profile.greybits <= palettebits))
        {
          unsigned grey = profile.key_r;
          mode_out->colortype = LCT_GREY;
          mode_out->bitdepth = profile.greybits;
          if(profile.key) setColorKeyFrom16bit(mode_out, grey, grey, grey, mode_out->bitdepth);
        }
        else
        {
          /*fill in the palette*/
          unsigned i;
          unsigned char* p = profile.palette;
          /*remove potential earlier palette*/
          lodepng_palette_clear(mode_out);
          for(i = 0; i < profile.numcolors; i++)
          {
            error = lodepng_palette_add(mode_out, p[i * 4 + 0], p[i * 4 + 1], p[i * 4 + 2], p[i * 4 + 3]);
            if(error) break;
          }

          mode_out->colortype = LCT_PALETTE;
          mode_out->bitdepth = palettebits;
        }
      }
      else /*8-bit per channel*/
      {
        mode_out->bitdepth = 8;
        if(profile.alpha)
        {
          mode_out->colortype = profile.colored ? LCT_RGBA : LCT_GREY_ALPHA;
        }
        else
        {
          mode_out->colortype = profile.colored ? LCT_RGB : LCT_GREY /*LCT_GREY normally won't occur, already done earlier*/;
          if(profile.key) setColorKeyFrom16bit(mode_out, profile.key_r, profile.key_g, profile.key_b, mode_out->bitdepth);
        }
      }
    }
  }

  color_profile_cleanup(&profile);

  if(mode_out->colortype == LCT_PALETTE && mode_in->palettesize == mode_out->palettesize)
  {
    /*In this case keep the palette order of the input, so that the user can choose an optimal one*/
    size_t i;
    for(i = 0; i < mode_in->palettesize * 4; i++)
    {
      mode_out->palette[i] = mode_in->palette[i];
    }
  }

  if(no_nibbles && mode_out->bitdepth < 8)
  {
    /*palette can keep its small amount of colors, as long as no indices use it*/
    mode_out->bitdepth = 8;
  }

  return error;
}

#endif /* #ifdef LODEPNG_COMPILE_ENCODER */

/*
Paeth predicter, used by PNG filter type 4
The parameters are of type short, but should come from unsigned chars, the shorts
are only needed to make the paeth calculation correct.
*/
static unsigned char paethPredictor(short a, short b, short c)
{
  short pa = abs(b - c);
  short pb = abs(a - c);
  short pc = abs(a + b - c - c);

  if(pc < pa && pc < pb) return (unsigned char)c;
  else if(pb < pa) return (unsigned char)b;
  else return (unsigned char)a;
}

/*shared values used by multiple Adam7 related functions*/

static const unsigned ADAM7_IX[7] = { 0, 4, 0, 2, 0, 1, 0 }; /*x start values*/
static const unsigned ADAM7_IY[7] = { 0, 0, 4, 0, 2, 0, 1 }; /*y start values*/
static const unsigned ADAM7_DX[7] = { 8, 8, 4, 4, 2, 2, 1 }; /*x delta values*/
static const unsigned ADAM7_DY[7] = { 8, 8, 8, 4, 4, 2, 2 }; /*y delta values*/

/*
Outputs various dimensions and positions in the image related to the Adam7 reduced images.
passw: output containing the width of the 7 passes
passh: output containing the height of the 7 passes
filter_passstart: output containing the index of the start and end of each
 reduced image with filter bytes
padded_passstart output containing the index of the start and end of each
 reduced image when without filter bytes but with padded scanlines
passstart: output containing the index of the start and end of each reduced
 image without padding between scanlines, but still padding between the images
w, h: width and height of non-interlaced image
bpp: bits per pixel
"padded" is only relevant if bpp is less than 8 and a scanline or image does not
 end at a full byte
*/
static void Adam7_getpassvalues(unsigned passw[7], unsigned passh[7], size_t filter_passstart[8],
                                size_t padded_passstart[8], size_t passstart[8], unsigned w, unsigned h, unsigned bpp)
{
  /*the passstart values have 8 values: the 8th one indicates the byte after the end of the 7th (= last) pass*/
  unsigned i;

  /*calculate width and height in pixels of each pass*/
  for(i = 0; i < 7; i++)
  {
    passw[i] = (w + ADAM7_DX[i] - ADAM7_IX[i] - 1) / ADAM7_DX[i];
    passh[i] = (h + ADAM7_DY[i] - ADAM7_IY[i] - 1) / ADAM7_DY[i];
    if(passw[i] == 0) passh[i] = 0;
    if(passh[i] == 0) passw[i] = 0;
  }

  filter_passstart[0] = padded_passstart[0] = passstart[0] = 0;
  for(i = 0; i < 7; i++)
  {
    /*if passw[i] is 0, it's 0 bytes, not 1 (no filtertype-byte)*/
    filter_passstart[i + 1] = filter_passstart[i]
                            + ((passw[i] && passh[i]) ? passh[i] * (1 + (passw[i] * bpp + 7) / 8) : 0);
    /*bits padded if needed to fill full byte at end of each scanline*/
    padded_passstart[i + 1] = padded_passstart[i] + passh[i] * ((passw[i] * bpp + 7) / 8);
    /*only padded at end of reduced image*/
    passstart[i + 1] = passstart[i] + (passh[i] * passw[i] * bpp + 7) / 8;
  }
}

#ifdef LODEPNG_COMPILE_DECODER

/* ////////////////////////////////////////////////////////////////////////// */
/* / PNG Decoder                                                            / */
/* ////////////////////////////////////////////////////////////////////////// */

/*read the information from the header and store it in the LodePNGInfo. return value is error*/
unsigned lodepng_inspect(unsigned* w, unsigned* h, LodePNGState* state,
                         const unsigned char* in, size_t insize)
{
  LodePNGInfo* info = &state->info_png;
  if(insize == 0 || in == 0)
  {
    CERROR_RETURN_ERROR(state->error, 48); /*error: the given data is empty*/
  }
  if(insize < 29)
  {
    CERROR_RETURN_ERROR(state->error, 27); /*error: the data length is smaller than the length of a PNG header*/
  }

  /*when decoding a new PNG image, make sure all parameters created after previous decoding are reset*/
  lodepng_info_cleanup(info);
  lodepng_info_init(info);

  if(in[0] != 137 || in[1] != 80 || in[2] != 78 || in[3] != 71
     || in[4] != 13 || in[5] != 10 || in[6] != 26 || in[7] != 10)
  {
    CERROR_RETURN_ERROR(state->error, 28); /*error: the first 8 bytes are not the correct PNG signature*/
  }
  if(in[12] != 'I' || in[13] != 'H' || in[14] != 'D' || in[15] != 'R')
  {
    CERROR_RETURN_ERROR(state->error, 29); /*error: it doesn't start with a IHDR chunk!*/
  }

  /*read the values given in the header*/
  *w = lodepng_read32bitInt(&in[16]);
  *h = lodepng_read32bitInt(&in[20]);
  info->color.bitdepth = in[24];
  info->color.colortype = (LodePNGColorType)in[25];
  info->compression_method = in[26];
  info->filter_method = in[27];
  info->interlace_method = in[28];

  if(!state->decoder.ignore_crc)
  {
    unsigned CRC = lodepng_read32bitInt(&in[29]);
    unsigned checksum = lodepng_crc32(&in[12], 17);
    if(CRC != checksum)
    {
      CERROR_RETURN_ERROR(state->error, 57); /*invalid CRC*/
    }
  }

  /*error: only compression method 0 is allowed in the specification*/
  if(info->compression_method != 0) CERROR_RETURN_ERROR(state->error, 32);
  /*error: only filter method 0 is allowed in the specification*/
  if(info->filter_method != 0) CERROR_RETURN_ERROR(state->error, 33);
  /*error: only interlace methods 0 and 1 exist in the specification*/
  if(info->interlace_method > 1) CERROR_RETURN_ERROR(state->error, 34);

  state->error = checkColorValidity(info->color.colortype, info->color.bitdepth);
  return state->error;
}

static unsigned unfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon,
                                 size_t bytewidth, unsigned char filterType, size_t length)
{
  /*
  For PNG filter method 0
  unfilter a PNG image scanline by scanline. when the pixels are smaller than 1 byte,
  the filter works byte per byte (bytewidth = 1)
  precon is the previous unfiltered scanline, recon the result, scanline the current one
  the incoming scanlines do NOT include the filtertype byte, that one is given in the parameter filterType instead
  recon and scanline MAY be the same memory address! precon must be disjoint.
  */

  size_t i;
  switch(filterType)
  {
    case 0:
      for(i = 0; i < length; i++) recon[i] = scanline[i];
      break;
    case 1:
      for(i = 0; i < bytewidth; i++) recon[i] = scanline[i];
      for(i = bytewidth; i < length; i++) recon[i] = scanline[i] + recon[i - bytewidth];
      break;
    case 2:
      if(precon)
      {
        for(i = 0; i < length; i++) recon[i] = scanline[i] + precon[i];
      }
      else
      {
        for(i = 0; i < length; i++) recon[i] = scanline[i];
      }
      break;
    case 3:
      if(precon)
      {
        for(i = 0; i < bytewidth; i++) recon[i] = scanline[i] + precon[i] / 2;
        for(i = bytewidth; i < length; i++) recon[i] = scanline[i] + ((recon[i - bytewidth] + precon[i]) / 2);
      }
      else
      {
        for(i = 0; i < bytewidth; i++) recon[i] = scanline[i];
        for(i = bytewidth; i < length; i++) recon[i] = scanline[i] + recon[i - bytewidth] / 2;
      }
      break;
    case 4:
      if(precon)
      {
        for(i = 0; i < bytewidth; i++)
        {
          recon[i] = (scanline[i] + precon[i]); /*paethPredictor(0, precon[i], 0) is always precon[i]*/
        }
        for(i = bytewidth; i < length; i++)
        {
          recon[i] = (scanline[i] + paethPredictor(recon[i - bytewidth], precon[i], precon[i - bytewidth]));
        }
      }
      else
      {
        for(i = 0; i < bytewidth; i++)
        {
          recon[i] = scanline[i];
        }
        for(i = bytewidth; i < length; i++)
        {
          /*paethPredictor(recon[i - bytewidth], 0, 0) is always recon[i - bytewidth]*/
          recon[i] = (scanline[i] + recon[i - bytewidth]);
        }
      }
      break;
    default: return 36; /*error: unexisting filter type given*/
  }
  return 0;
}

static unsigned unfilter(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp)
{
  /*
  For PNG filter method 0
  this function unfilters a single image (e.g. without interlacing this is called once, with Adam7 seven times)
  out must have enough bytes allocated already, in must have the scanlines + 1 filtertype byte per scanline
  w and h are image dimensions or dimensions of reduced image, bpp is bits per pixel
  in and out are allowed to be the same memory address (but aren't the same size since in has the extra filter bytes)
  */

  unsigned y;
  unsigned char* prevline = 0;

  /*bytewidth is used for filtering, is 1 when bpp < 8, number of bytes per pixel otherwise*/
  size_t bytewidth = (bpp + 7) / 8;
  size_t linebytes = (w * bpp + 7) / 8;

  for(y = 0; y < h; y++)
  {
    size_t outindex = linebytes * y;
    size_t inindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
    unsigned char filterType = in[inindex];

    CERROR_TRY_RETURN(unfilterScanline(&out[outindex], &in[inindex + 1], prevline, bytewidth, filterType, linebytes));

    prevline = &out[outindex];
  }

  return 0;
}

/*
in: Adam7 interlaced image, with no padding bits between scanlines, but between
 reduced images so that each reduced image starts at a byte.
out: the same pixels, but re-ordered so that they're now a non-interlaced image with size w*h
bpp: bits per pixel
out has the following size in bits: w * h * bpp.
in is possibly bigger due to padding bits between reduced images.
out must be big enough AND must be 0 everywhere if bpp < 8 in the current implementation
(because that's likely a little bit faster)
NOTE: comments about padding bits are only relevant if bpp < 8
*/
static void Adam7_deinterlace(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp)
{
  unsigned passw[7], passh[7];
  size_t filter_passstart[8], padded_passstart[8], passstart[8];
  unsigned i;

  Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

  if(bpp >= 8)
  {
    for(i = 0; i < 7; i++)
    {
      unsigned x, y, b;
      size_t bytewidth = bpp / 8;
      for(y = 0; y < passh[i]; y++)
      for(x = 0; x < passw[i]; x++)
      {
        size_t pixelinstart = passstart[i] + (y * passw[i] + x) * bytewidth;
        size_t pixeloutstart = ((ADAM7_IY[i] + y * ADAM7_DY[i]) * w + ADAM7_IX[i] + x * ADAM7_DX[i]) * bytewidth;
        for(b = 0; b < bytewidth; b++)
        {
          out[pixeloutstart + b] = in[pixelinstart + b];
        }
      }
    }
  }
  else /*bpp < 8: Adam7 with pixels < 8 bit is a bit trickier: with bit pointers*/
  {
    for(i = 0; i < 7; i++)
    {
      unsigned x, y, b;
      unsigned ilinebits = bpp * passw[i];
      unsigned olinebits = bpp * w;
      size_t obp, ibp; /*bit pointers (for out and in buffer)*/
      for(y = 0; y < passh[i]; y++)
      for(x = 0; x < passw[i]; x++)
      {
        ibp = (8 * passstart[i]) + (y * ilinebits + x * bpp);
        obp = (ADAM7_IY[i] + y * ADAM7_DY[i]) * olinebits + (ADAM7_IX[i] + x * ADAM7_DX[i]) * bpp;
        for(b = 0; b < bpp; b++)
        {
          unsigned char bit = readBitFromReversedStream(&ibp, in);
          /*note that this function assumes the out buffer is completely 0, use setBitOfReversedStream otherwise*/
          setBitOfReversedStream0(&obp, out, bit);
        }
      }
    }
  }
}

static void removePaddingBits(unsigned char* out, const unsigned char* in,
                              size_t olinebits, size_t ilinebits, unsigned h)
{
  /*
  After filtering there are still padding bits if scanlines have non multiple of 8 bit amounts. They need
  to be removed (except at last scanline of (Adam7-reduced) image) before working with pure image buffers
  for the Adam7 code, the color convert code and the output to the user.
  in and out are allowed to be the same buffer, in may also be higher but still overlapping; in must
  have >= ilinebits*h bits, out must have >= olinebits*h bits, olinebits must be <= ilinebits
  also used to move bits after earlier such operations happened, e.g. in a sequence of reduced images from Adam7
  only useful if (ilinebits - olinebits) is a value in the range 1..7
  */
  unsigned y;
  size_t diff = ilinebits - olinebits;
  size_t ibp = 0, obp = 0; /*input and output bit pointers*/
  for(y = 0; y < h; y++)
  {
    size_t x;
    for(x = 0; x < olinebits; x++)
    {
      unsigned char bit = readBitFromReversedStream(&ibp, in);
      setBitOfReversedStream(&obp, out, bit);
    }
    ibp += diff;
  }
}

/*out must be buffer big enough to contain full image, and in must contain the full decompressed data from
the IDAT chunks (with filter index bytes and possible padding bits)
return value is error*/
static unsigned postProcessScanlines(unsigned char* out, unsigned char* in,
                                     unsigned w, unsigned h, const LodePNGInfo* info_png)
{
  /*
  This function converts the filtered-padded-interlaced data into pure 2D image buffer with the PNG's colortype.
  Steps:
  *) if no Adam7: 1) unfilter 2) remove padding bits (= posible extra bits per scanline if bpp < 8)
  *) if adam7: 1) 7x unfilter 2) 7x remove padding bits 3) Adam7_deinterlace
  NOTE: the in buffer will be overwritten with intermediate data!
  */
  unsigned bpp = lodepng_get_bpp(&info_png->color);
  if(bpp == 0) return 31; /*error: invalid colortype*/

  if(info_png->interlace_method == 0)
  {
    if(bpp < 8 && w * bpp != ((w * bpp + 7) / 8) * 8)
    {
      CERROR_TRY_RETURN(unfilter(in, in, w, h, bpp));
      removePaddingBits(out, in, w * bpp, ((w * bpp + 7) / 8) * 8, h);
    }
    /*we can immediatly filter into the out buffer, no other steps needed*/
    else CERROR_TRY_RETURN(unfilter(out, in, w, h, bpp));
  }
  else /*interlace_method is 1 (Adam7)*/
  {
    unsigned passw[7], passh[7]; size_t filter_passstart[8], padded_passstart[8], passstart[8];
    unsigned i;

    Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

    for(i = 0; i < 7; i++)
    {
      CERROR_TRY_RETURN(unfilter(&in[padded_passstart[i]], &in[filter_passstart[i]], passw[i], passh[i], bpp));
      /*TODO: possible efficiency improvement: if in this reduced image the bits fit nicely in 1 scanline,
      move bytes instead of bits or move not at all*/
      if(bpp < 8)
      {
        /*remove padding bits in scanlines; after this there still may be padding
        bits between the different reduced images: each reduced image still starts nicely at a byte*/
        removePaddingBits(&in[passstart[i]], &in[padded_passstart[i]], passw[i] * bpp,
                          ((passw[i] * bpp + 7) / 8) * 8, passh[i]);
      }
    }

    Adam7_deinterlace(out, in, w, h, bpp);
  }

  return 0;
}

static unsigned readChunk_PLTE(LodePNGColorMode* color, const unsigned char* data, size_t chunkLength)
{
  unsigned pos = 0, i;
  if(color->palette) lodepng_free(color->palette);
  color->palettesize = chunkLength / 3;
  color->palette = (unsigned char*)lodepng_malloc(4 * color->palettesize);
  if(!color->palette && color->palettesize)
  {
    color->palettesize = 0;
    return 83; /*alloc fail*/
  }
  if(color->palettesize > 256) return 38; /*error: palette too big*/

  for(i = 0; i < color->palettesize; i++)
  {
    color->palette[4 * i + 0] = data[pos++]; /*R*/
    color->palette[4 * i + 1] = data[pos++]; /*G*/
    color->palette[4 * i + 2] = data[pos++]; /*B*/
    color->palette[4 * i + 3] = 255; /*alpha*/
  }

  return 0; /* OK */
}

static unsigned readChunk_tRNS(LodePNGColorMode* color, const unsigned char* data, size_t chunkLength)
{
  unsigned i;
  if(color->colortype == LCT_PALETTE)
  {
    /*error: more alpha values given than there are palette entries*/
    if(chunkLength > color->palettesize) return 38;

    for(i = 0; i < chunkLength; i++) color->palette[4 * i + 3] = data[i];
  }
  else if(color->colortype == LCT_GREY)
  {
    /*error: this chunk must be 2 bytes for greyscale image*/
    if(chunkLength != 2) return 30;

    color->key_defined = 1;
    color->key_r = color->key_g = color->key_b = 256 * data[0] + data[1];
  }
  else if(color->colortype == LCT_RGB)
  {
    /*error: this chunk must be 6 bytes for RGB image*/
    if(chunkLength != 6) return 41;

    color->key_defined = 1;
    color->key_r = 256 * data[0] + data[1];
    color->key_g = 256 * data[2] + data[3];
    color->key_b = 256 * data[4] + data[5];
  }
  else return 42; /*error: tRNS chunk not allowed for other color models*/

  return 0; /* OK */
}


#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
/*background color chunk (bKGD)*/
static unsigned readChunk_bKGD(LodePNGInfo* info, const unsigned char* data, size_t chunkLength)
{
  if(info->color.colortype == LCT_PALETTE)
  {
    /*error: this chunk must be 1 byte for indexed color image*/
    if(chunkLength != 1) return 43;

    info->background_defined = 1;
    info->background_r = info->background_g = info->background_b = data[0];
  }
  else if(info->color.colortype == LCT_GREY || info->color.colortype == LCT_GREY_ALPHA)
  {
    /*error: this chunk must be 2 bytes for greyscale image*/
    if(chunkLength != 2) return 44;

    info->background_defined = 1;
    info->background_r = info->background_g = info->background_b
                                 = 256 * data[0] + data[1];
  }
  else if(info->color.colortype == LCT_RGB || info->color.colortype == LCT_RGBA)
  {
    /*error: this chunk must be 6 bytes for greyscale image*/
    if(chunkLength != 6) return 45;

    info->background_defined = 1;
    info->background_r = 256 * data[0] + data[1];
    info->background_g = 256 * data[2] + data[3];
    info->background_b = 256 * data[4] + data[5];
  }

  return 0; /* OK */
}

/*text chunk (tEXt)*/
static unsigned readChunk_tEXt(LodePNGInfo* info, const unsigned char* data, size_t chunkLength)
{
  unsigned error = 0;
  char *key = 0, *str = 0;
  unsigned i;

  while(!error) /*not really a while loop, only used to break on error*/
  {
    unsigned length, string2_begin;

    length = 0;
    while(length < chunkLength && data[length] != 0) length++;
    /*even though it's not allowed by the standard, no error is thrown if
    there's no null termination char, if the text is empty*/
    if(length < 1 || length > 79) CERROR_BREAK(error, 89); /*keyword too short or long*/

    key = (char*)lodepng_malloc(length + 1);
    if(!key) CERROR_BREAK(error, 83); /*alloc fail*/

    key[length] = 0;
    for(i = 0; i < length; i++) key[i] = data[i];

    string2_begin = length + 1; /*skip keyword null terminator*/

    length = chunkLength < string2_begin ? 0 : chunkLength - string2_begin;
    str = (char*)lodepng_malloc(length + 1);
    if(!str) CERROR_BREAK(error, 83); /*alloc fail*/

    str[length] = 0;
    for(i = 0; i < length; i++) str[i] = data[string2_begin + i];

    error = lodepng_add_text(info, key, str);

    break;
  }

  lodepng_free(key);
  lodepng_free(str);

  return error;
}

/*compressed text chunk (zTXt)*/
static unsigned readChunk_zTXt(LodePNGInfo* info, const LodePNGDecompressSettings* zlibsettings,
                               const unsigned char* data, size_t chunkLength)
{
  unsigned error = 0;
  unsigned i;

  unsigned length, string2_begin;
  char *key = 0;
  ucvector decoded;

  ucvector_init(&decoded);

  while(!error) /*not really a while loop, only used to break on error*/
  {
    for(length = 0; length < chunkLength && data[length] != 0; length++) ;
    if(length + 2 >= chunkLength) CERROR_BREAK(error, 75); /*no null termination, corrupt?*/
    if(length < 1 || length > 79) CERROR_BREAK(error, 89); /*keyword too short or long*/

    key = (char*)lodepng_malloc(length + 1);
    if(!key) CERROR_BREAK(error, 83); /*alloc fail*/

    key[length] = 0;
    for(i = 0; i < length; i++) key[i] = data[i];

    if(data[length + 1] != 0) CERROR_BREAK(error, 72); /*the 0 byte indicating compression must be 0*/

    string2_begin = length + 2;
    if(string2_begin > chunkLength) CERROR_BREAK(error, 75); /*no null termination, corrupt?*/

    length = chunkLength - string2_begin;
    /*will fail if zlib error, e.g. if length is too small*/
    error = zlib_decompress(&decoded.data, &decoded.size,
                            (unsigned char*)(&data[string2_begin]),
                            length, zlibsettings);
    if(error) break;
    ucvector_push_back(&decoded, 0);

    error = lodepng_add_text(info, key, (char*)decoded.data);

    break;
  }

  lodepng_free(key);
  ucvector_cleanup(&decoded);

  return error;
}

/*international text chunk (iTXt)*/
static unsigned readChunk_iTXt(LodePNGInfo* info, const LodePNGDecompressSettings* zlibsettings,
                               const unsigned char* data, size_t chunkLength)
{
  unsigned error = 0;
  unsigned i;

  unsigned length, begin, compressed;
  char *key = 0, *langtag = 0, *transkey = 0;
  ucvector decoded;
  ucvector_init(&decoded);

  while(!error) /*not really a while loop, only used to break on error*/
  {
    /*Quick check if the chunk length isn't too small. Even without check
    it'd still fail with other error checks below if it's too short. This just gives a different error code.*/
    if(chunkLength < 5) CERROR_BREAK(error, 30); /*iTXt chunk too short*/

    /*read the key*/
    for(length = 0; length < chunkLength && data[length] != 0; length++) ;
    if(length + 3 >= chunkLength) CERROR_BREAK(error, 75); /*no null termination char, corrupt?*/
    if(length < 1 || length > 79) CERROR_BREAK(error, 89); /*keyword too short or long*/

    key = (char*)lodepng_malloc(length + 1);
    if(!key) CERROR_BREAK(error, 83); /*alloc fail*/

    key[length] = 0;
    for(i = 0; i < length; i++) key[i] = data[i];

    /*read the compression method*/
    compressed = data[length + 1];
    if(data[length + 2] != 0) CERROR_BREAK(error, 72); /*the 0 byte indicating compression must be 0*/

    /*even though it's not allowed by the standard, no error is thrown if
    there's no null termination char, if the text is empty for the next 3 texts*/

    /*read the langtag*/
    begin = length + 3;
    length = 0;
    for(i = begin; i < chunkLength && data[i] != 0; i++) length++;

    langtag = (char*)lodepng_malloc(length + 1);
    if(!langtag) CERROR_BREAK(error, 83); /*alloc fail*/

    langtag[length] = 0;
    for(i = 0; i < length; i++) langtag[i] = data[begin + i];

    /*read the transkey*/
    begin += length + 1;
    length = 0;
    for(i = begin; i < chunkLength && data[i] != 0; i++) length++;

    transkey = (char*)lodepng_malloc(length + 1);
    if(!transkey) CERROR_BREAK(error, 83); /*alloc fail*/

    transkey[length] = 0;
    for(i = 0; i < length; i++) transkey[i] = data[begin + i];

    /*read the actual text*/
    begin += length + 1;

    length = chunkLength < begin ? 0 : chunkLength - begin;

    if(compressed)
    {
      /*will fail if zlib error, e.g. if length is too small*/
      error = zlib_decompress(&decoded.data, &decoded.size,
                              (unsigned char*)(&data[begin]),
                              length, zlibsettings);
      if(error) break;
      if(decoded.allocsize < decoded.size) decoded.allocsize = decoded.size;
      ucvector_push_back(&decoded, 0);
    }
    else
    {
      if(!ucvector_resize(&decoded, length + 1)) CERROR_BREAK(error, 83 /*alloc fail*/);

      decoded.data[length] = 0;
      for(i = 0; i < length; i++) decoded.data[i] = data[begin + i];
    }

    error = lodepng_add_itext(info, key, langtag, transkey, (char*)decoded.data);

    break;
  }

  lodepng_free(key);
  lodepng_free(langtag);
  lodepng_free(transkey);
  ucvector_cleanup(&decoded);

  return error;
}

static unsigned readChunk_tIME(LodePNGInfo* info, const unsigned char* data, size_t chunkLength)
{
  if(chunkLength != 7) return 73; /*invalid tIME chunk size*/

  info->time_defined = 1;
  info->time.year = 256 * data[0] + data[+ 1];
  info->time.month = data[2];
  info->time.day = data[3];
  info->time.hour = data[4];
  info->time.minute = data[5];
  info->time.second = data[6];

  return 0; /* OK */
}

static unsigned readChunk_pHYs(LodePNGInfo* info, const unsigned char* data, size_t chunkLength)
{
  if(chunkLength != 9) return 74; /*invalid pHYs chunk size*/

  info->phys_defined = 1;
  info->phys_x = 16777216 * data[0] + 65536 * data[1] + 256 * data[2] + data[3];
  info->phys_y = 16777216 * data[4] + 65536 * data[5] + 256 * data[6] + data[7];
  info->phys_unit = data[8];

  return 0; /* OK */
}
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

/*read a PNG, the result will be in the same color type as the PNG (hence "generic")*/
static void decodeGeneric(unsigned char** out, unsigned* w, unsigned* h,
                          LodePNGState* state,
                          const unsigned char* in, size_t insize)
{
  unsigned char IEND = 0;
  const unsigned char* chunk;
  size_t i;
  ucvector idat; /*the data from idat chunks*/

  /*for unknown chunk order*/
  unsigned unknown = 0;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  unsigned critical_pos = 1; /*1 = after IHDR, 2 = after PLTE, 3 = after IDAT*/
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

  /*provide some proper output values if error will happen*/
  *out = 0;

  state->error = lodepng_inspect(w, h, state, in, insize); /*reads header and resets other parameters in state->info_png*/
  if(state->error) return;

  ucvector_init(&idat);
  chunk = &in[33]; /*first byte of the first chunk after the header*/

  /*loop through the chunks, ignoring unknown chunks and stopping at IEND chunk.
  IDAT data is put at the start of the in buffer*/
  while(!IEND && !state->error)
  {
    unsigned chunkLength;
    const unsigned char* data; /*the data in the chunk*/

    /*error: size of the in buffer too small to contain next chunk*/
    if((size_t)((chunk - in) + 12) > insize || chunk < in) CERROR_BREAK(state->error, 30);

    /*length of the data of the chunk, excluding the length bytes, chunk type and CRC bytes*/
    chunkLength = lodepng_chunk_length(chunk);
    /*error: chunk length larger than the max PNG chunk size*/
    if(chunkLength > 2147483647) CERROR_BREAK(state->error, 63);

    if((size_t)((chunk - in) + chunkLength + 12) > insize || (chunk + chunkLength + 12) < in)
    {
      CERROR_BREAK(state->error, 64); /*error: size of the in buffer too small to contain next chunk*/
    }

    data = lodepng_chunk_data_const(chunk);

    /*IDAT chunk, containing compressed image data*/
    if(lodepng_chunk_type_equals(chunk, "IDAT"))
    {
      size_t oldsize = idat.size;
      if(!ucvector_resize(&idat, oldsize + chunkLength)) CERROR_BREAK(state->error, 83 /*alloc fail*/);
      for(i = 0; i < chunkLength; i++) idat.data[oldsize + i] = data[i];
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
      critical_pos = 3;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    }
    /*IEND chunk*/
    else if(lodepng_chunk_type_equals(chunk, "IEND"))
    {
      IEND = 1;
    }
    /*palette chunk (PLTE)*/
    else if(lodepng_chunk_type_equals(chunk, "PLTE"))
    {
      state->error = readChunk_PLTE(&state->info_png.color, data, chunkLength);
      if(state->error) break;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
      critical_pos = 2;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    }
    /*palette transparency chunk (tRNS)*/
    else if(lodepng_chunk_type_equals(chunk, "tRNS"))
    {
      state->error = readChunk_tRNS(&state->info_png.color, data, chunkLength);
      if(state->error) break;
    }
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    /*background color chunk (bKGD)*/
    else if(lodepng_chunk_type_equals(chunk, "bKGD"))
    {
      state->error = readChunk_bKGD(&state->info_png, data, chunkLength);
      if(state->error) break;
    }
    /*text chunk (tEXt)*/
    else if(lodepng_chunk_type_equals(chunk, "tEXt"))
    {
      if(state->decoder.read_text_chunks)
      {
        state->error = readChunk_tEXt(&state->info_png, data, chunkLength);
        if(state->error) break;
      }
    }
    /*compressed text chunk (zTXt)*/
    else if(lodepng_chunk_type_equals(chunk, "zTXt"))
    {
      if(state->decoder.read_text_chunks)
      {
        state->error = readChunk_zTXt(&state->info_png, &state->decoder.zlibsettings, data, chunkLength);
        if(state->error) break;
      }
    }
    /*international text chunk (iTXt)*/
    else if(lodepng_chunk_type_equals(chunk, "iTXt"))
    {
      if(state->decoder.read_text_chunks)
      {
        state->error = readChunk_iTXt(&state->info_png, &state->decoder.zlibsettings, data, chunkLength);
        if(state->error) break;
      }
    }
    else if(lodepng_chunk_type_equals(chunk, "tIME"))
    {
      state->error = readChunk_tIME(&state->info_png, data, chunkLength);
      if(state->error) break;
    }
    else if(lodepng_chunk_type_equals(chunk, "pHYs"))
    {
      state->error = readChunk_pHYs(&state->info_png, data, chunkLength);
      if(state->error) break;
    }
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    else /*it's not an implemented chunk type, so ignore it: skip over the data*/
    {
      /*error: unknown critical chunk (5th bit of first byte of chunk type is 0)*/
      if(!lodepng_chunk_ancillary(chunk)) CERROR_BREAK(state->error, 69);

      unknown = 1;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
      if(state->decoder.remember_unknown_chunks)
      {
        state->error = lodepng_chunk_append(&state->info_png.unknown_chunks_data[critical_pos - 1],
                                            &state->info_png.unknown_chunks_size[critical_pos - 1], chunk);
        if(state->error) break;
      }
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    }

    if(!state->decoder.ignore_crc && !unknown) /*check CRC if wanted, only on known chunk types*/
    {
      if(lodepng_chunk_check_crc(chunk)) CERROR_BREAK(state->error, 57); /*invalid CRC*/
    }

    if(!IEND) chunk = lodepng_chunk_next_const(chunk);
  }

  if(!state->error)
  {
    ucvector scanlines;
    ucvector_init(&scanlines);

    /*maximum final image length is already reserved in the vector's length - this is not really necessary*/
    if(!ucvector_resize(&scanlines, lodepng_get_raw_size(*w, *h, &state->info_png.color) + *h))
    {
      state->error = 83; /*alloc fail*/
    }
    if(!state->error)
    {
      /*decompress with the Zlib decompressor*/
      state->error = zlib_decompress(&scanlines.data, &scanlines.size, idat.data,
                                     idat.size, &state->decoder.zlibsettings);
    }

    if(!state->error)
    {
      ucvector outv;
      ucvector_init(&outv);
      if(!ucvector_resizev(&outv,
          lodepng_get_raw_size(*w, *h, &state->info_png.color), 0)) state->error = 83; /*alloc fail*/
      if(!state->error) state->error = postProcessScanlines(outv.data, scanlines.data, *w, *h, &state->info_png);
      *out = outv.data;
    }
    ucvector_cleanup(&scanlines);
  }

  ucvector_cleanup(&idat);
}

unsigned lodepng_decode(unsigned char** out, unsigned* w, unsigned* h,
                        LodePNGState* state,
                        const unsigned char* in, size_t insize)
{
  *out = 0;
  decodeGeneric(out, w, h, state, in, insize);
  if(state->error) return state->error;
  if(!state->decoder.color_convert || lodepng_color_mode_equal(&state->info_raw, &state->info_png.color))
  {
    /*same color type, no copying or converting of data needed*/
    /*store the info_png color settings on the info_raw so that the info_raw still reflects what colortype
    the raw image has to the end user*/
    if(!state->decoder.color_convert)
    {
      state->error = lodepng_color_mode_copy(&state->info_raw, &state->info_png.color);
      if(state->error) return state->error;
    }
  }
  else
  {
    /*color conversion needed; sort of copy of the data*/
    unsigned char* data = *out;
    size_t outsize;

    /*TODO: check if this works according to the statement in the documentation: "The converter can convert
    from greyscale input color type, to 8-bit greyscale or greyscale with alpha"*/
    if(!(state->info_raw.colortype == LCT_RGB || state->info_raw.colortype == LCT_RGBA)
       && !(state->info_raw.bitdepth == 8))
    {
      return 56; /*unsupported color mode conversion*/
    }

    outsize = lodepng_get_raw_size(*w, *h, &state->info_raw);
    *out = (unsigned char*)lodepng_malloc(outsize);
    if(!(*out))
    {
      state->error = 83; /*alloc fail*/
    }
    else state->error = lodepng_convert(*out, data, &state->info_raw, &state->info_png.color, *w, *h, state->decoder.fix_png);
    lodepng_free(data);
  }
  return state->error;
}

unsigned lodepng_decode_memory(unsigned char** out, unsigned* w, unsigned* h, const unsigned char* in,
                               size_t insize, LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned error;
  LodePNGState state;
  lodepng_state_init(&state);
  state.info_raw.colortype = colortype;
  state.info_raw.bitdepth = bitdepth;
  error = lodepng_decode(out, w, h, &state, in, insize);
  lodepng_state_cleanup(&state);
  return error;
}

unsigned lodepng_decode32(unsigned char** out, unsigned* w, unsigned* h, const unsigned char* in, size_t insize)
{
  return lodepng_decode_memory(out, w, h, in, insize, LCT_RGBA, 8);
}

unsigned lodepng_decode24(unsigned char** out, unsigned* w, unsigned* h, const unsigned char* in, size_t insize)
{
  return lodepng_decode_memory(out, w, h, in, insize, LCT_RGB, 8);
}

#ifdef LODEPNG_COMPILE_DISK
unsigned lodepng_decode_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename,
                             LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error;
  error = lodepng_load_file(&buffer, &buffersize, filename);
  if(!error) error = lodepng_decode_memory(out, w, h, buffer, buffersize, colortype, bitdepth);
  lodepng_free(buffer);
  return error;
}

unsigned lodepng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename)
{
  return lodepng_decode_file(out, w, h, filename, LCT_RGBA, 8);
}

unsigned lodepng_decode24_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename)
{
  return lodepng_decode_file(out, w, h, filename, LCT_RGB, 8);
}
#endif /*LODEPNG_COMPILE_DISK*/

void lodepng_decoder_settings_init(LodePNGDecoderSettings* settings)
{
  settings->color_convert = 1;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  settings->read_text_chunks = 1;
  settings->remember_unknown_chunks = 0;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
  settings->ignore_crc = 0;
  settings->fix_png = 0;
  lodepng_decompress_settings_init(&settings->zlibsettings);
}

#endif /*LODEPNG_COMPILE_DECODER*/

#if defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER)

void lodepng_state_init(LodePNGState* state)
{
#ifdef LODEPNG_COMPILE_DECODER
  lodepng_decoder_settings_init(&state->decoder);
#endif /*LODEPNG_COMPILE_DECODER*/
#ifdef LODEPNG_COMPILE_ENCODER
  lodepng_encoder_settings_init(&state->encoder);
#endif /*LODEPNG_COMPILE_ENCODER*/
  lodepng_color_mode_init(&state->info_raw);
  lodepng_info_init(&state->info_png);
  state->error = 1;
}

void lodepng_state_cleanup(LodePNGState* state)
{
  lodepng_color_mode_cleanup(&state->info_raw);
  lodepng_info_cleanup(&state->info_png);
}

void lodepng_state_copy(LodePNGState* dest, const LodePNGState* source)
{
  lodepng_state_cleanup(dest);
  *dest = *source;
  lodepng_color_mode_init(&dest->info_raw);
  lodepng_info_init(&dest->info_png);
  dest->error = lodepng_color_mode_copy(&dest->info_raw, &source->info_raw); if(dest->error) return;
  dest->error = lodepng_info_copy(&dest->info_png, &source->info_png); if(dest->error) return;
}

#endif /* defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER) */

#ifdef LODEPNG_COMPILE_ENCODER

/* ////////////////////////////////////////////////////////////////////////// */
/* / PNG Encoder                                                            / */
/* ////////////////////////////////////////////////////////////////////////// */

/*chunkName must be string of 4 characters*/
static unsigned addChunk(ucvector* out, const char* chunkName, const unsigned char* data, size_t length)
{
  CERROR_TRY_RETURN(lodepng_chunk_create(&out->data, &out->size, (unsigned)length, chunkName, data));
  out->allocsize = out->size; /*fix the allocsize again*/
  return 0;
}

static void writeSignature(ucvector* out)
{
  /*8 bytes PNG signature, aka the magic bytes*/
  ucvector_push_back(out, 137);
  ucvector_push_back(out, 80);
  ucvector_push_back(out, 78);
  ucvector_push_back(out, 71);
  ucvector_push_back(out, 13);
  ucvector_push_back(out, 10);
  ucvector_push_back(out, 26);
  ucvector_push_back(out, 10);
}

static unsigned addChunk_IHDR(ucvector* out, unsigned w, unsigned h,
                              LodePNGColorType colortype, unsigned bitdepth, unsigned interlace_method)
{
  unsigned error = 0;
  ucvector header;
  ucvector_init(&header);

  lodepng_add32bitInt(&header, w); /*width*/
  lodepng_add32bitInt(&header, h); /*height*/
  ucvector_push_back(&header, (unsigned char)bitdepth); /*bit depth*/
  ucvector_push_back(&header, (unsigned char)colortype); /*color type*/
  ucvector_push_back(&header, 0); /*compression method*/
  ucvector_push_back(&header, 0); /*filter method*/
  ucvector_push_back(&header, interlace_method); /*interlace method*/

  error = addChunk(out, "IHDR", header.data, header.size);
  ucvector_cleanup(&header);

  return error;
}

static unsigned addChunk_PLTE(ucvector* out, const LodePNGColorMode* info)
{
  unsigned error = 0;
  size_t i;
  ucvector PLTE;
  ucvector_init(&PLTE);
  for(i = 0; i < info->palettesize * 4; i++)
  {
    /*add all channels except alpha channel*/
    if(i % 4 != 3) ucvector_push_back(&PLTE, info->palette[i]);
  }
  error = addChunk(out, "PLTE", PLTE.data, PLTE.size);
  ucvector_cleanup(&PLTE);

  return error;
}

static unsigned addChunk_tRNS(ucvector* out, const LodePNGColorMode* info)
{
  unsigned error = 0;
  size_t i;
  ucvector tRNS;
  ucvector_init(&tRNS);
  if(info->colortype == LCT_PALETTE)
  {
    size_t amount = info->palettesize;
    /*the tail of palette values that all have 255 as alpha, does not have to be encoded*/
    for(i = info->palettesize; i > 0; i--)
    {
      if(info->palette[4 * (i - 1) + 3] == 255) amount--;
      else break;
    }
    /*add only alpha channel*/
    for(i = 0; i < amount; i++) ucvector_push_back(&tRNS, info->palette[4 * i + 3]);
  }
  else if(info->colortype == LCT_GREY)
  {
    if(info->key_defined)
    {
      ucvector_push_back(&tRNS, (unsigned char)(info->key_r / 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_r % 256));
    }
  }
  else if(info->colortype == LCT_RGB)
  {
    if(info->key_defined)
    {
      ucvector_push_back(&tRNS, (unsigned char)(info->key_r / 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_r % 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_g / 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_g % 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_b / 256));
      ucvector_push_back(&tRNS, (unsigned char)(info->key_b % 256));
    }
  }

  error = addChunk(out, "tRNS", tRNS.data, tRNS.size);
  ucvector_cleanup(&tRNS);

  return error;
}

static unsigned addChunk_IDAT(ucvector* out, const unsigned char* data, size_t datasize,
                              LodePNGCompressSettings* zlibsettings)
{
  ucvector zlibdata;
  unsigned error = 0;

  /*compress with the Zlib compressor*/
  ucvector_init(&zlibdata);
  error = zlib_compress(&zlibdata.data, &zlibdata.size, data, datasize, zlibsettings);
  if(!error) error = addChunk(out, "IDAT", zlibdata.data, zlibdata.size);
  ucvector_cleanup(&zlibdata);

  return error;
}

static unsigned addChunk_IEND(ucvector* out)
{
  unsigned error = 0;
  error = addChunk(out, "IEND", 0, 0);
  return error;
}

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

static unsigned addChunk_tEXt(ucvector* out, const char* keyword, const char* textstring)
{
  unsigned error = 0;
  size_t i;
  ucvector text;
  ucvector_init(&text);
  for(i = 0; keyword[i] != 0; i++) ucvector_push_back(&text, (unsigned char)keyword[i]);
  if(i < 1 || i > 79) return 89; /*error: invalid keyword size*/
  ucvector_push_back(&text, 0); /*0 termination char*/
  for(i = 0; textstring[i] != 0; i++) ucvector_push_back(&text, (unsigned char)textstring[i]);
  error = addChunk(out, "tEXt", text.data, text.size);
  ucvector_cleanup(&text);

  return error;
}

static unsigned addChunk_zTXt(ucvector* out, const char* keyword, const char* textstring,
                              LodePNGCompressSettings* zlibsettings)
{
  unsigned error = 0;
  ucvector data, compressed;
  size_t i, textsize = strlen(textstring);

  ucvector_init(&data);
  ucvector_init(&compressed);
  for(i = 0; keyword[i] != 0; i++) ucvector_push_back(&data, (unsigned char)keyword[i]);
  if(i < 1 || i > 79) return 89; /*error: invalid keyword size*/
  ucvector_push_back(&data, 0); /*0 termination char*/
  ucvector_push_back(&data, 0); /*compression method: 0*/

  error = zlib_compress(&compressed.data, &compressed.size,
                        (unsigned char*)textstring, textsize, zlibsettings);
  if(!error)
  {
    for(i = 0; i < compressed.size; i++) ucvector_push_back(&data, compressed.data[i]);
    error = addChunk(out, "zTXt", data.data, data.size);
  }

  ucvector_cleanup(&compressed);
  ucvector_cleanup(&data);
  return error;
}

static unsigned addChunk_iTXt(ucvector* out, unsigned compressed, const char* keyword, const char* langtag,
                              const char* transkey, const char* textstring, LodePNGCompressSettings* zlibsettings)
{
  unsigned error = 0;
  ucvector data;
  size_t i, textsize = strlen(textstring);

  ucvector_init(&data);

  for(i = 0; keyword[i] != 0; i++) ucvector_push_back(&data, (unsigned char)keyword[i]);
  if(i < 1 || i > 79) return 89; /*error: invalid keyword size*/
  ucvector_push_back(&data, 0); /*null termination char*/
  ucvector_push_back(&data, compressed ? 1 : 0); /*compression flag*/
  ucvector_push_back(&data, 0); /*compression method*/
  for(i = 0; langtag[i] != 0; i++) ucvector_push_back(&data, (unsigned char)langtag[i]);
  ucvector_push_back(&data, 0); /*null termination char*/
  for(i = 0; transkey[i] != 0; i++) ucvector_push_back(&data, (unsigned char)transkey[i]);
  ucvector_push_back(&data, 0); /*null termination char*/

  if(compressed)
  {
    ucvector compressed_data;
    ucvector_init(&compressed_data);
    error = zlib_compress(&compressed_data.data, &compressed_data.size,
                          (unsigned char*)textstring, textsize, zlibsettings);
    if(!error)
    {
      for(i = 0; i < compressed_data.size; i++) ucvector_push_back(&data, compressed_data.data[i]);
    }
    ucvector_cleanup(&compressed_data);
  }
  else /*not compressed*/
  {
    for(i = 0; textstring[i] != 0; i++) ucvector_push_back(&data, (unsigned char)textstring[i]);
  }

  if(!error) error = addChunk(out, "iTXt", data.data, data.size);
  ucvector_cleanup(&data);
  return error;
}

static unsigned addChunk_bKGD(ucvector* out, const LodePNGInfo* info)
{
  unsigned error = 0;
  ucvector bKGD;
  ucvector_init(&bKGD);
  if(info->color.colortype == LCT_GREY || info->color.colortype == LCT_GREY_ALPHA)
  {
    ucvector_push_back(&bKGD, (unsigned char)(info->background_r / 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_r % 256));
  }
  else if(info->color.colortype == LCT_RGB || info->color.colortype == LCT_RGBA)
  {
    ucvector_push_back(&bKGD, (unsigned char)(info->background_r / 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_r % 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_g / 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_g % 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_b / 256));
    ucvector_push_back(&bKGD, (unsigned char)(info->background_b % 256));
  }
  else if(info->color.colortype == LCT_PALETTE)
  {
    ucvector_push_back(&bKGD, (unsigned char)(info->background_r % 256)); /*palette index*/
  }

  error = addChunk(out, "bKGD", bKGD.data, bKGD.size);
  ucvector_cleanup(&bKGD);

  return error;
}

static unsigned addChunk_tIME(ucvector* out, const LodePNGTime* time)
{
  unsigned error = 0;
  unsigned char* data = (unsigned char*)lodepng_malloc(7);
  if(!data) return 83; /*alloc fail*/
  data[0] = (unsigned char)(time->year / 256);
  data[1] = (unsigned char)(time->year % 256);
  data[2] = time->month;
  data[3] = time->day;
  data[4] = time->hour;
  data[5] = time->minute;
  data[6] = time->second;
  error = addChunk(out, "tIME", data, 7);
  lodepng_free(data);
  return error;
}

static unsigned addChunk_pHYs(ucvector* out, const LodePNGInfo* info)
{
  unsigned error = 0;
  ucvector data;
  ucvector_init(&data);

  lodepng_add32bitInt(&data, info->phys_x);
  lodepng_add32bitInt(&data, info->phys_y);
  ucvector_push_back(&data, info->phys_unit);

  error = addChunk(out, "pHYs", data.data, data.size);
  ucvector_cleanup(&data);

  return error;
}

#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

static void filterScanline(unsigned char* out, const unsigned char* scanline, const unsigned char* prevline,
                           size_t length, size_t bytewidth, unsigned char filterType)
{
  size_t i;
  switch(filterType)
  {
    case 0: /*None*/
      for(i = 0; i < length; i++) out[i] = scanline[i];
      break;
    case 1: /*Sub*/
      if(prevline)
      {
        for(i = 0; i < bytewidth; i++) out[i] = scanline[i];
        for(i = bytewidth; i < length; i++) out[i] = scanline[i] - scanline[i - bytewidth];
      }
      else
      {
        for(i = 0; i < bytewidth; i++) out[i] = scanline[i];
        for(i = bytewidth; i < length; i++) out[i] = scanline[i] - scanline[i - bytewidth];
      }
      break;
    case 2: /*Up*/
      if(prevline)
      {
        for(i = 0; i < length; i++) out[i] = scanline[i] - prevline[i];
      }
      else
      {
        for(i = 0; i < length; i++) out[i] = scanline[i];
      }
      break;
    case 3: /*Average*/
      if(prevline)
      {
        for(i = 0; i < bytewidth; i++) out[i] = scanline[i] - prevline[i] / 2;
        for(i = bytewidth; i < length; i++) out[i] = scanline[i] - ((scanline[i - bytewidth] + prevline[i]) / 2);
      }
      else
      {
        for(i = 0; i < bytewidth; i++) out[i] = scanline[i];
        for(i = bytewidth; i < length; i++) out[i] = scanline[i] - scanline[i - bytewidth] / 2;
      }
      break;
    case 4: /*Paeth*/
      if(prevline)
      {
        /*paethPredictor(0, prevline[i], 0) is always prevline[i]*/
        for(i = 0; i < bytewidth; i++) out[i] = (scanline[i] - prevline[i]);
        for(i = bytewidth; i < length; i++)
        {
          out[i] = (scanline[i] - paethPredictor(scanline[i - bytewidth], prevline[i], prevline[i - bytewidth]));
        }
      }
      else
      {
        for(i = 0; i < bytewidth; i++) out[i] = scanline[i];
        /*paethPredictor(scanline[i - bytewidth], 0, 0) is always scanline[i - bytewidth]*/
        for(i = bytewidth; i < length; i++) out[i] = (scanline[i] - scanline[i - bytewidth]);
      }
      break;
    default: return; /*unexisting filter type given*/
  }
}

/* log2 approximation. A slight bit faster than std::log. */
static float flog2(float f)
{
  float result = 0;
  while(f > 32) { result += 4; f /= 16; }
  while(f > 2) { result++; f /= 2; }
  return result + 1.442695f * (f * f * f / 3 - 3 * f * f / 2 + 3 * f - 1.83333f);
}

static unsigned filter(unsigned char* out, const unsigned char* in, unsigned w, unsigned h,
                       const LodePNGColorMode* info, const LodePNGEncoderSettings* settings)
{
  /*
  For PNG filter method 0
  out must be a buffer with as size: h + (w * h * bpp + 7) / 8, because there are
  the scanlines with 1 extra byte per scanline
  */

  unsigned bpp = lodepng_get_bpp(info);
  /*the width of a scanline in bytes, not including the filter type*/
  size_t linebytes = (w * bpp + 7) / 8;
  /*bytewidth is used for filtering, is 1 when bpp < 8, number of bytes per pixel otherwise*/
  size_t bytewidth = (bpp + 7) / 8;
  const unsigned char* prevline = 0;
  unsigned x, y;
  unsigned error = 0;
  LodePNGFilterStrategy strategy = settings->filter_strategy;

  /*
  There is a heuristic called the minimum sum of absolute differences heuristic, suggested by the PNG standard:
   *  If the image type is Palette, or the bit depth is smaller than 8, then do not filter the image (i.e.
      use fixed filtering, with the filter None).
   * (The other case) If the image type is Grayscale or RGB (with or without Alpha), and the bit depth is
     not smaller than 8, then use adaptive filtering heuristic as follows: independently for each row, apply
     all five filters and select the filter that produces the smallest sum of absolute values per row.
  This heuristic is used if filter strategy is LFS_MINSUM and filter_palette_zero is true.

  If filter_palette_zero is true and filter_strategy is not LFS_MINSUM, the above heuristic is followed,
  but for "the other case", whatever strategy filter_strategy is set to instead of the minimum sum
  heuristic is used.
  */
  if(settings->filter_palette_zero &&
     (info->colortype == LCT_PALETTE || info->bitdepth < 8)) strategy = LFS_ZERO;

  if(bpp == 0) return 31; /*error: invalid color type*/

  if(strategy == LFS_ZERO)
  {
    for(y = 0; y < h; y++)
    {
      size_t outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
      size_t inindex = linebytes * y;
      out[outindex] = 0; /*filter type byte*/
      filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, 0);
      prevline = &in[inindex];
    }
  }
  else if(strategy == LFS_MINSUM)
  {
    /*adaptive filtering*/
    size_t sum[5];
    ucvector attempt[5]; /*five filtering attempts, one for each filter type*/
    size_t smallest = 0;
    unsigned type, bestType = 0;

    for(type = 0; type < 5; type++)
    {
      ucvector_init(&attempt[type]);
      if(!ucvector_resize(&attempt[type], linebytes)) return 83; /*alloc fail*/
    }

    if(!error)
    {
      for(y = 0; y < h; y++)
      {
        /*try the 5 filter types*/
        for(type = 0; type < 5; type++)
        {
          filterScanline(attempt[type].data, &in[y * linebytes], prevline, linebytes, bytewidth, type);

          /*calculate the sum of the result*/
          sum[type] = 0;
          if(type == 0)
          {
            for(x = 0; x < linebytes; x++) sum[type] += (unsigned char)(attempt[type].data[x]);
          }
          else
          {
            for(x = 0; x < linebytes; x++)
            {
              /*For differences, each byte should be treated as signed, values above 127 are negative
              (converted to signed char). Filtertype 0 isn't a difference though, so use unsigned there.
              This means filtertype 0 is almost never chosen, but that is justified.*/
              signed char s = (signed char)(attempt[type].data[x]);
              sum[type] += s < 0 ? -s : s;
            }
          }

          /*check if this is smallest sum (or if type == 0 it's the first case so always store the values)*/
          if(type == 0 || sum[type] < smallest)
          {
            bestType = type;
            smallest = sum[type];
          }
        }

        prevline = &in[y * linebytes];

        /*now fill the out values*/
        out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
        for(x = 0; x < linebytes; x++) out[y * (linebytes + 1) + 1 + x] = attempt[bestType].data[x];
      }
    }

    for(type = 0; type < 5; type++) ucvector_cleanup(&attempt[type]);
  }
  else if(strategy == LFS_ENTROPY)
  {
    float sum[5];
    ucvector attempt[5]; /*five filtering attempts, one for each filter type*/
    float smallest = 0;
    unsigned type, bestType = 0;
    unsigned count[256];

    for(type = 0; type < 5; type++)
    {
      ucvector_init(&attempt[type]);
      if(!ucvector_resize(&attempt[type], linebytes)) return 83; /*alloc fail*/
    }

    for(y = 0; y < h; y++)
    {
      /*try the 5 filter types*/
      for(type = 0; type < 5; type++)
      {
        filterScanline(attempt[type].data, &in[y * linebytes], prevline, linebytes, bytewidth, type);
        for(x = 0; x < 256; x++) count[x] = 0;
        for(x = 0; x < linebytes; x++) count[attempt[type].data[x]]++;
        count[type]++; /*the filter type itself is part of the scanline*/
        sum[type] = 0;
        for(x = 0; x < 256; x++)
        {
          float p = count[x] / (float)(linebytes + 1);
          sum[type] += count[x] == 0 ? 0 : flog2(1 / p) * p;
        }
        /*check if this is smallest sum (or if type == 0 it's the first case so always store the values)*/
        if(type == 0 || sum[type] < smallest)
        {
          bestType = type;
          smallest = sum[type];
        }
      }

      prevline = &in[y * linebytes];

      /*now fill the out values*/
      out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
      for(x = 0; x < linebytes; x++) out[y * (linebytes + 1) + 1 + x] = attempt[bestType].data[x];
    }

    for(type = 0; type < 5; type++) ucvector_cleanup(&attempt[type]);
  }
  else if(strategy == LFS_PREDEFINED)
  {
    for(y = 0; y < h; y++)
    {
      size_t outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
      size_t inindex = linebytes * y;
      unsigned type = settings->predefined_filters[y];
      out[outindex] = type; /*filter type byte*/
      filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, type);
      prevline = &in[inindex];
    }
  }
  else if(strategy == LFS_BRUTE_FORCE)
  {
    /*brute force filter chooser.
    deflate the scanline after every filter attempt to see which one deflates best.
    This is very slow and gives only slightly smaller, sometimes even larger, result*/
    size_t size[5];
    ucvector attempt[5]; /*five filtering attempts, one for each filter type*/
    size_t smallest = 0;
    unsigned type = 0, bestType = 0;
    unsigned char* dummy;
    LodePNGCompressSettings zlibsettings = settings->zlibsettings;
    /*use fixed tree on the attempts so that the tree is not adapted to the filtertype on purpose,
    to simulate the true case where the tree is the same for the whole image. Sometimes it gives
    better result with dynamic tree anyway. Using the fixed tree sometimes gives worse, but in rare
    cases better compression. It does make this a bit less slow, so it's worth doing this.*/
    zlibsettings.btype = 1;
    /*a custom encoder likely doesn't read the btype setting and is optimized for complete PNG
    images only, so disable it*/
    zlibsettings.custom_zlib = 0;
    zlibsettings.custom_deflate = 0;
    for(type = 0; type < 5; type++)
    {
      ucvector_init(&attempt[type]);
      ucvector_resize(&attempt[type], linebytes); /*todo: give error if resize failed*/
    }
    for(y = 0; y < h; y++) /*try the 5 filter types*/
    {
      for(type = 0; type < 5; type++)
      {
        unsigned testsize = attempt[type].size;
        /*if(testsize > 8) testsize /= 8;*/ /*it already works good enough by testing a part of the row*/

        filterScanline(attempt[type].data, &in[y * linebytes], prevline, linebytes, bytewidth, type);
        size[type] = 0;
        dummy = 0;
        zlib_compress(&dummy, &size[type], attempt[type].data, testsize, &zlibsettings);
        lodepng_free(dummy);
        /*check if this is smallest size (or if type == 0 it's the first case so always store the values)*/
        if(type == 0 || size[type] < smallest)
        {
          bestType = type;
          smallest = size[type];
        }
      }
      prevline = &in[y * linebytes];
      out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
      for(x = 0; x < linebytes; x++) out[y * (linebytes + 1) + 1 + x] = attempt[bestType].data[x];
    }
    for(type = 0; type < 5; type++) ucvector_cleanup(&attempt[type]);
  }
  else return 88; /* unknown filter strategy */

  return error;
}

static void addPaddingBits(unsigned char* out, const unsigned char* in,
                           size_t olinebits, size_t ilinebits, unsigned h)
{
  /*The opposite of the removePaddingBits function
  olinebits must be >= ilinebits*/
  unsigned y;
  size_t diff = olinebits - ilinebits;
  size_t obp = 0, ibp = 0; /*bit pointers*/
  for(y = 0; y < h; y++)
  {
    size_t x;
    for(x = 0; x < ilinebits; x++)
    {
      unsigned char bit = readBitFromReversedStream(&ibp, in);
      setBitOfReversedStream(&obp, out, bit);
    }
    /*obp += diff; --> no, fill in some value in the padding bits too, to avoid
    "Use of uninitialised value of size ###" warning from valgrind*/
    for(x = 0; x < diff; x++) setBitOfReversedStream(&obp, out, 0);
  }
}

/*
in: non-interlaced image with size w*h
out: the same pixels, but re-ordered according to PNG's Adam7 interlacing, with
 no padding bits between scanlines, but between reduced images so that each
 reduced image starts at a byte.
bpp: bits per pixel
there are no padding bits, not between scanlines, not between reduced images
in has the following size in bits: w * h * bpp.
out is possibly bigger due to padding bits between reduced images
NOTE: comments about padding bits are only relevant if bpp < 8
*/
static void Adam7_interlace(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp)
{
  unsigned passw[7], passh[7];
  size_t filter_passstart[8], padded_passstart[8], passstart[8];
  unsigned i;

  Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

  if(bpp >= 8)
  {
    for(i = 0; i < 7; i++)
    {
      unsigned x, y, b;
      size_t bytewidth = bpp / 8;
      for(y = 0; y < passh[i]; y++)
      for(x = 0; x < passw[i]; x++)
      {
        size_t pixelinstart = ((ADAM7_IY[i] + y * ADAM7_DY[i]) * w + ADAM7_IX[i] + x * ADAM7_DX[i]) * bytewidth;
        size_t pixeloutstart = passstart[i] + (y * passw[i] + x) * bytewidth;
        for(b = 0; b < bytewidth; b++)
        {
          out[pixeloutstart + b] = in[pixelinstart + b];
        }
      }
    }
  }
  else /*bpp < 8: Adam7 with pixels < 8 bit is a bit trickier: with bit pointers*/
  {
    for(i = 0; i < 7; i++)
    {
      unsigned x, y, b;
      unsigned ilinebits = bpp * passw[i];
      unsigned olinebits = bpp * w;
      size_t obp, ibp; /*bit pointers (for out and in buffer)*/
      for(y = 0; y < passh[i]; y++)
      for(x = 0; x < passw[i]; x++)
      {
        ibp = (ADAM7_IY[i] + y * ADAM7_DY[i]) * olinebits + (ADAM7_IX[i] + x * ADAM7_DX[i]) * bpp;
        obp = (8 * passstart[i]) + (y * ilinebits + x * bpp);
        for(b = 0; b < bpp; b++)
        {
          unsigned char bit = readBitFromReversedStream(&ibp, in);
          setBitOfReversedStream(&obp, out, bit);
        }
      }
    }
  }
}

/*out must be buffer big enough to contain uncompressed IDAT chunk data, and in must contain the full image.
return value is error**/
static unsigned preProcessScanlines(unsigned char** out, size_t* outsize, const unsigned char* in,
                                    unsigned w, unsigned h,
                                    const LodePNGInfo* info_png, const LodePNGEncoderSettings* settings)
{
  /*
  This function converts the pure 2D image with the PNG's colortype, into filtered-padded-interlaced data. Steps:
  *) if no Adam7: 1) add padding bits (= posible extra bits per scanline if bpp < 8) 2) filter
  *) if adam7: 1) Adam7_interlace 2) 7x add padding bits 3) 7x filter
  */
  unsigned bpp = lodepng_get_bpp(&info_png->color);
  unsigned error = 0;

  if(info_png->interlace_method == 0)
  {
    *outsize = h + (h * ((w * bpp + 7) / 8)); /*image size plus an extra byte per scanline + possible padding bits*/
    *out = (unsigned char*)lodepng_malloc(*outsize);
    if(!(*out) && (*outsize)) error = 83; /*alloc fail*/

    if(!error)
    {
      /*non multiple of 8 bits per scanline, padding bits needed per scanline*/
      if(bpp < 8 && w * bpp != ((w * bpp + 7) / 8) * 8)
      {
        unsigned char* padded = (unsigned char*)lodepng_malloc(h * ((w * bpp + 7) / 8));
        if(!padded) error = 83; /*alloc fail*/
        if(!error)
        {
          addPaddingBits(padded, in, ((w * bpp + 7) / 8) * 8, w * bpp, h);
          error = filter(*out, padded, w, h, &info_png->color, settings);
        }
        lodepng_free(padded);
      }
      else
      {
        /*we can immediatly filter into the out buffer, no other steps needed*/
        error = filter(*out, in, w, h, &info_png->color, settings);
      }
    }
  }
  else /*interlace_method is 1 (Adam7)*/
  {
    unsigned passw[7], passh[7];
    size_t filter_passstart[8], padded_passstart[8], passstart[8];
    unsigned char* adam7;

    Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

    *outsize = filter_passstart[7]; /*image size plus an extra byte per scanline + possible padding bits*/
    *out = (unsigned char*)lodepng_malloc(*outsize);
    if(!(*out)) error = 83; /*alloc fail*/

    adam7 = (unsigned char*)lodepng_malloc(passstart[7]);
    if(!adam7 && passstart[7]) error = 83; /*alloc fail*/

    if(!error)
    {
      unsigned i;

      Adam7_interlace(adam7, in, w, h, bpp);
      for(i = 0; i < 7; i++)
      {
        if(bpp < 8)
        {
          unsigned char* padded = (unsigned char*)lodepng_malloc(padded_passstart[i + 1] - padded_passstart[i]);
          if(!padded) ERROR_BREAK(83); /*alloc fail*/
          addPaddingBits(padded, &adam7[passstart[i]],
                         ((passw[i] * bpp + 7) / 8) * 8, passw[i] * bpp, passh[i]);
          error = filter(&(*out)[filter_passstart[i]], padded,
                         passw[i], passh[i], &info_png->color, settings);
          lodepng_free(padded);
        }
        else
        {
          error = filter(&(*out)[filter_passstart[i]], &adam7[padded_passstart[i]],
                         passw[i], passh[i], &info_png->color, settings);
        }

        if(error) break;
      }
    }

    lodepng_free(adam7);
  }

  return error;
}

/*
palette must have 4 * palettesize bytes allocated, and given in format RGBARGBARGBARGBA...
returns 0 if the palette is opaque,
returns 1 if the palette has a single color with alpha 0 ==> color key
returns 2 if the palette is semi-translucent.
*/
static unsigned getPaletteTranslucency(const unsigned char* palette, size_t palettesize)
{
  size_t i, key = 0;
  unsigned r = 0, g = 0, b = 0; /*the value of the color with alpha 0, so long as color keying is possible*/
  for(i = 0; i < palettesize; i++)
  {
    if(!key && palette[4 * i + 3] == 0)
    {
      r = palette[4 * i + 0]; g = palette[4 * i + 1]; b = palette[4 * i + 2];
      key = 1;
      i = (size_t)(-1); /*restart from beginning, to detect earlier opaque colors with key's value*/
    }
    else if(palette[4 * i + 3] != 255) return 2;
    /*when key, no opaque RGB may have key's RGB*/
    else if(key && r == palette[i * 4 + 0] && g == palette[i * 4 + 1] && b == palette[i * 4 + 2]) return 2;
  }
  return key;
}

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
static unsigned addUnknownChunks(ucvector* out, unsigned char* data, size_t datasize)
{
  unsigned char* inchunk = data;
  while((size_t)(inchunk - data) < datasize)
  {
    CERROR_TRY_RETURN(lodepng_chunk_append(&out->data, &out->size, inchunk));
    out->allocsize = out->size; /*fix the allocsize again*/
    inchunk = lodepng_chunk_next(inchunk);
  }
  return 0;
}
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

unsigned lodepng_encode(unsigned char** out, size_t* outsize,
                        const unsigned char* image, unsigned w, unsigned h,
                        LodePNGState* state)
{
  LodePNGInfo info;
  ucvector outv;
  unsigned char* data = 0; /*uncompressed version of the IDAT chunk data*/
  size_t datasize = 0;

  /*provide some proper output values if error will happen*/
  *out = 0;
  *outsize = 0;
  state->error = 0;

  lodepng_info_init(&info);
  lodepng_info_copy(&info, &state->info_png);

  if((info.color.colortype == LCT_PALETTE || state->encoder.force_palette)
      && (info.color.palettesize == 0 || info.color.palettesize > 256))
  {
    state->error = 68; /*invalid palette size, it is only allowed to be 1-256*/
    return state->error;
  }

  if(state->encoder.auto_convert != LAC_NO)
  {
    state->error = doAutoChooseColor(&info.color, image, w, h, &state->info_raw,
                                     state->encoder.auto_convert);
  }
  if(state->error) return state->error;

  if(state->encoder.zlibsettings.windowsize > 32768)
  {
    CERROR_RETURN_ERROR(state->error, 60); /*error: windowsize larger than allowed*/
  }
  if(state->encoder.zlibsettings.btype > 2)
  {
    CERROR_RETURN_ERROR(state->error, 61); /*error: unexisting btype*/
  }
  if(state->info_png.interlace_method > 1)
  {
    CERROR_RETURN_ERROR(state->error, 71); /*error: unexisting interlace mode*/
  }

  state->error = checkColorValidity(info.color.colortype, info.color.bitdepth);
  if(state->error) return state->error; /*error: unexisting color type given*/
  state->error = checkColorValidity(state->info_raw.colortype, state->info_raw.bitdepth);
  if(state->error) return state->error; /*error: unexisting color type given*/

  if(!lodepng_color_mode_equal(&state->info_raw, &info.color))
  {
    unsigned char* converted;
    size_t size = (w * h * lodepng_get_bpp(&info.color) + 7) / 8;

    converted = (unsigned char*)lodepng_malloc(size);
    if(!converted && size) state->error = 83; /*alloc fail*/
    if(!state->error)
    {
      state->error = lodepng_convert(converted, image, &info.color, &state->info_raw, w, h, 0 /*fix_png*/);
    }
    if(!state->error) preProcessScanlines(&data, &datasize, converted, w, h, &info, &state->encoder);
    lodepng_free(converted);
  }
  else preProcessScanlines(&data, &datasize, image, w, h, &info, &state->encoder);

  ucvector_init(&outv);
  while(!state->error) /*while only executed once, to break on error*/
  {
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    size_t i;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    /*write signature and chunks*/
    writeSignature(&outv);
    /*IHDR*/
    addChunk_IHDR(&outv, w, h, info.color.colortype, info.color.bitdepth, info.interlace_method);
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    /*unknown chunks between IHDR and PLTE*/
    if(info.unknown_chunks_data[0])
    {
      state->error = addUnknownChunks(&outv, info.unknown_chunks_data[0], info.unknown_chunks_size[0]);
      if(state->error) break;
    }
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    /*PLTE*/
    if(info.color.colortype == LCT_PALETTE)
    {
      addChunk_PLTE(&outv, &info.color);
    }
    if(state->encoder.force_palette && (info.color.colortype == LCT_RGB || info.color.colortype == LCT_RGBA))
    {
      addChunk_PLTE(&outv, &info.color);
    }
    /*tRNS*/
    if(info.color.colortype == LCT_PALETTE && getPaletteTranslucency(info.color.palette, info.color.palettesize) != 0)
    {
      addChunk_tRNS(&outv, &info.color);
    }
    if((info.color.colortype == LCT_GREY || info.color.colortype == LCT_RGB) && info.color.key_defined)
    {
      addChunk_tRNS(&outv, &info.color);
    }
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    /*bKGD (must come between PLTE and the IDAt chunks*/
    if(info.background_defined) addChunk_bKGD(&outv, &info);
    /*pHYs (must come before the IDAT chunks)*/
    if(info.phys_defined) addChunk_pHYs(&outv, &info);

    /*unknown chunks between PLTE and IDAT*/
    if(info.unknown_chunks_data[1])
    {
      state->error = addUnknownChunks(&outv, info.unknown_chunks_data[1], info.unknown_chunks_size[1]);
      if(state->error) break;
    }
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    /*IDAT (multiple IDAT chunks must be consecutive)*/
    state->error = addChunk_IDAT(&outv, data, datasize, &state->encoder.zlibsettings);
    if(state->error) break;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    /*tIME*/
    if(info.time_defined) addChunk_tIME(&outv, &info.time);
    /*tEXt and/or zTXt*/
    for(i = 0; i < info.text_num; i++)
    {
      if(strlen(info.text_keys[i]) > 79)
      {
        state->error = 66; /*text chunk too large*/
        break;
      }
      if(strlen(info.text_keys[i]) < 1)
      {
        state->error = 67; /*text chunk too small*/
        break;
      }
      if(state->encoder.text_compression)
        addChunk_zTXt(&outv, info.text_keys[i], info.text_strings[i], &state->encoder.zlibsettings);
      else
        addChunk_tEXt(&outv, info.text_keys[i], info.text_strings[i]);
    }
    /*LodePNG version id in text chunk*/
    if(state->encoder.add_id)
    {
      unsigned alread_added_id_text = 0;
      for(i = 0; i < info.text_num; i++)
      {
        if(!strcmp(info.text_keys[i], "LodePNG"))
        {
          alread_added_id_text = 1;
          break;
        }
      }
      if(alread_added_id_text == 0)
        addChunk_tEXt(&outv, "LodePNG", VERSION_STRING); /*it's shorter as tEXt than as zTXt chunk*/
    }
    /*iTXt*/
    for(i = 0; i < info.itext_num; i++)
    {
      if(strlen(info.itext_keys[i]) > 79)
      {
        state->error = 66; /*text chunk too large*/
        break;
      }
      if(strlen(info.itext_keys[i]) < 1)
      {
        state->error = 67; /*text chunk too small*/
        break;
      }
      addChunk_iTXt(&outv, state->encoder.text_compression,
                    info.itext_keys[i], info.itext_langtags[i], info.itext_transkeys[i], info.itext_strings[i],
                    &state->encoder.zlibsettings);
    }

    /*unknown chunks between IDAT and IEND*/
    if(info.unknown_chunks_data[2])
    {
      state->error = addUnknownChunks(&outv, info.unknown_chunks_data[2], info.unknown_chunks_size[2]);
      if(state->error) break;
    }
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
    /*IEND*/
    addChunk_IEND(&outv);

    break; /*this isn't really a while loop; no error happened so break out now!*/
  }

  lodepng_info_cleanup(&info);
  lodepng_free(data);
  /*instead of cleaning the vector up, give it to the output*/
  *out = outv.data;
  *outsize = outv.size;

  return state->error;
}

unsigned lodepng_encode_memory(unsigned char** out, size_t* outsize, const unsigned char* image,
                               unsigned w, unsigned h, LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned error;
  LodePNGState state;
  lodepng_state_init(&state);
  state.info_raw.colortype = colortype;
  state.info_raw.bitdepth = bitdepth;
  state.info_png.color.colortype = colortype;
  state.info_png.color.bitdepth = bitdepth;
  lodepng_encode(out, outsize, image, w, h, &state);
  error = state.error;
  lodepng_state_cleanup(&state);
  return error;
}

unsigned lodepng_encode32(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h)
{
  return lodepng_encode_memory(out, outsize, image, w, h, LCT_RGBA, 8);
}

unsigned lodepng_encode24(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h)
{
  return lodepng_encode_memory(out, outsize, image, w, h, LCT_RGB, 8);
}

#ifdef LODEPNG_COMPILE_DISK
unsigned lodepng_encode_file(const char* filename, const unsigned char* image, unsigned w, unsigned h,
                             LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error = lodepng_encode_memory(&buffer, &buffersize, image, w, h, colortype, bitdepth);
  if(!error) error = lodepng_save_file(buffer, buffersize, filename);
  lodepng_free(buffer);
  return error;
}

unsigned lodepng_encode32_file(const char* filename, const unsigned char* image, unsigned w, unsigned h)
{
  return lodepng_encode_file(filename, image, w, h, LCT_RGBA, 8);
}

unsigned lodepng_encode24_file(const char* filename, const unsigned char* image, unsigned w, unsigned h)
{
  return lodepng_encode_file(filename, image, w, h, LCT_RGB, 8);
}
#endif /*LODEPNG_COMPILE_DISK*/

void lodepng_encoder_settings_init(LodePNGEncoderSettings* settings)
{
  lodepng_compress_settings_init(&settings->zlibsettings);
  settings->filter_palette_zero = 1;
  settings->filter_strategy = LFS_MINSUM;
  settings->auto_convert = LAC_AUTO;
  settings->force_palette = 0;
  settings->predefined_filters = 0;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
  settings->add_id = 0;
  settings->text_compression = 1;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
}

#endif /*LODEPNG_COMPILE_ENCODER*/
#endif /*LODEPNG_COMPILE_PNG*/

#ifdef LODEPNG_COMPILE_ERROR_TEXT
/*
This returns the description of a numerical error code in English. This is also
the documentation of all the error codes.
*/
const char* lodepng_error_text(unsigned code)
{
  switch(code)
  {
    case 0: return "no error, everything went ok";
    case 1: return "nothing done yet"; /*the Encoder/Decoder has done nothing yet, error checking makes no sense yet*/
    case 10: return "end of input memory reached without huffman end code"; /*while huffman decoding*/
    case 11: return "error in code tree made it jump outside of huffman tree"; /*while huffman decoding*/
    case 13: return "problem while processing dynamic deflate block";
    case 14: return "problem while processing dynamic deflate block";
    case 15: return "problem while processing dynamic deflate block";
    case 16: return "unexisting code while processing dynamic deflate block";
    case 17: return "end of out buffer memory reached while inflating";
    case 18: return "invalid distance code while inflating";
    case 19: return "end of out buffer memory reached while inflating";
    case 20: return "invalid deflate block BTYPE encountered while decoding";
    case 21: return "NLEN is not ones complement of LEN in a deflate block";
     /*end of out buffer memory reached while inflating:
     This can happen if the inflated deflate data is longer than the amount of bytes required to fill up
     all the pixels of the image, given the color depth and image dimensions. Something that doesn't
     happen in a normal, well encoded, PNG image.*/
    case 22: return "end of out buffer memory reached while inflating";
    case 23: return "end of in buffer memory reached while inflating";
    case 24: return "invalid FCHECK in zlib header";
    case 25: return "invalid compression method in zlib header";
    case 26: return "FDICT encountered in zlib header while it's not used for PNG";
    case 27: return "PNG file is smaller than a PNG header";
    /*Checks the magic file header, the first 8 bytes of the PNG file*/
    case 28: return "incorrect PNG signature, it's no PNG or corrupted";
    case 29: return "first chunk is not the header chunk";
    case 30: return "chunk length too large, chunk broken off at end of file";
    case 31: return "illegal PNG color type or bpp";
    case 32: return "illegal PNG compression method";
    case 33: return "illegal PNG filter method";
    case 34: return "illegal PNG interlace method";
    case 35: return "chunk length of a chunk is too large or the chunk too small";
    case 36: return "illegal PNG filter type encountered";
    case 37: return "illegal bit depth for this color type given";
    case 38: return "the palette is too big"; /*more than 256 colors*/
    case 39: return "more palette alpha values given in tRNS chunk than there are colors in the palette";
    case 40: return "tRNS chunk has wrong size for greyscale image";
    case 41: return "tRNS chunk has wrong size for RGB image";
    case 42: return "tRNS chunk appeared while it was not allowed for this color type";
    case 43: return "bKGD chunk has wrong size for palette image";
    case 44: return "bKGD chunk has wrong size for greyscale image";
    case 45: return "bKGD chunk has wrong size for RGB image";
    /*Is the palette too small?*/
    case 46: return "a value in indexed image is larger than the palette size (bitdepth = 8)";
    /*Is the palette too small?*/
    case 47: return "a value in indexed image is larger than the palette size (bitdepth < 8)";
    /*the input data is empty, maybe a PNG file doesn't exist or is in the wrong path*/
    case 48: return "empty input or file doesn't exist";
    case 49: return "jumped past memory while generating dynamic huffman tree";
    case 50: return "jumped past memory while generating dynamic huffman tree";
    case 51: return "jumped past memory while inflating huffman block";
    case 52: return "jumped past memory while inflating";
    case 53: return "size of zlib data too small";
    case 54: return "repeat symbol in tree while there was no value symbol yet";
    /*jumped past tree while generating huffman tree, this could be when the
    tree will have more leaves than symbols after generating it out of the
    given lenghts. They call this an oversubscribed dynamic bit lengths tree in zlib.*/
    case 55: return "jumped past tree while generating huffman tree";
    case 56: return "given output image colortype or bitdepth not supported for color conversion";
    case 57: return "invalid CRC encountered (checking CRC can be disabled)";
    case 58: return "invalid ADLER32 encountered (checking ADLER32 can be disabled)";
    case 59: return "requested color conversion not supported";
    case 60: return "invalid window size given in the settings of the encoder (must be 0-32768)";
    case 61: return "invalid BTYPE given in the settings of the encoder (only 0, 1 and 2 are allowed)";
    /*LodePNG leaves the choice of RGB to greyscale conversion formula to the user.*/
    case 62: return "conversion from color to greyscale not supported";
    case 63: return "length of a chunk too long, max allowed for PNG is 2147483647 bytes per chunk"; /*(2^31-1)*/
    /*this would result in the inability of a deflated block to ever contain an end code. It must be at least 1.*/
    case 64: return "the length of the END symbol 256 in the Huffman tree is 0";
    case 66: return "the length of a text chunk keyword given to the encoder is longer than the maximum of 79 bytes";
    case 67: return "the length of a text chunk keyword given to the encoder is smaller than the minimum of 1 byte";
    case 68: return "tried to encode a PLTE chunk with a palette that has less than 1 or more than 256 colors";
    case 69: return "unknown chunk type with 'critical' flag encountered by the decoder";
    case 71: return "unexisting interlace mode given to encoder (must be 0 or 1)";
    case 72: return "while decoding, unexisting compression method encountering in zTXt or iTXt chunk (it must be 0)";
    case 73: return "invalid tIME chunk size";
    case 74: return "invalid pHYs chunk size";
    /*length could be wrong, or data chopped off*/
    case 75: return "no null termination char found while decoding text chunk";
    case 76: return "iTXt chunk too short to contain required bytes";
    case 77: return "integer overflow in buffer size";
    case 78: return "failed to open file for reading"; /*file doesn't exist or couldn't be opened for reading*/
    case 79: return "failed to open file for writing";
    case 80: return "tried creating a tree of 0 symbols";
    case 81: return "lazy matching at pos 0 is impossible";
    case 82: return "color conversion to palette requested while a color isn't in palette";
    case 83: return "memory allocation failed";
    case 84: return "given image too small to contain all pixels to be encoded";
    case 85: return "internal color conversion bug";
    case 86: return "impossible offset in lz77 encoding (internal bug)";
    case 87: return "must provide custom zlib function pointer if LODEPNG_COMPILE_ZLIB is not defined";
    case 88: return "invalid filter strategy given for LodePNGEncoderSettings.filter_strategy";
    case 89: return "text chunk keyword too short or long: must have size 1-79";
  }
  return "unknown error code";
}
#endif /*LODEPNG_COMPILE_ERROR_TEXT*/

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* // C++ Wrapper                                                          // */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */


#ifdef LODEPNG_COMPILE_CPP
namespace lodepng
{

#ifdef LODEPNG_COMPILE_DISK
void load_file(std::vector<unsigned char>& buffer, const std::string& filename)
{
  std::ifstream file(filename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

  /*get filesize*/
  std::streamsize size = 0;
  if(file.seekg(0, std::ios::end).good()) size = file.tellg();
  if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

  /*read contents of the file into the vector*/
  buffer.resize(size_t(size));
  if(size > 0) file.read((char*)(&buffer[0]), size);
}

/*write given buffer to the file, overwriting the file, it doesn't append to it.*/
void save_file(const std::vector<unsigned char>& buffer, const std::string& filename)
{
  std::ofstream file(filename.c_str(), std::ios::out|std::ios::binary);
  file.write(buffer.empty() ? 0 : (char*)&buffer[0], std::streamsize(buffer.size()));
}
#endif //LODEPNG_COMPILE_DISK

#ifdef LODEPNG_COMPILE_ZLIB
#ifdef LODEPNG_COMPILE_DECODER
unsigned decompress(std::vector<unsigned char>& out, const unsigned char* in, size_t insize,
                    const LodePNGDecompressSettings& settings)
{
  unsigned char* buffer = 0;
  size_t buffersize = 0;
  unsigned error = zlib_decompress(&buffer, &buffersize, in, insize, &settings);
  if(buffer)
  {
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned decompress(std::vector<unsigned char>& out, const std::vector<unsigned char>& in,
                    const LodePNGDecompressSettings& settings)
{
  return decompress(out, in.empty() ? 0 : &in[0], in.size(), settings);
}
#endif //LODEPNG_COMPILE_DECODER

#ifdef LODEPNG_COMPILE_ENCODER
unsigned compress(std::vector<unsigned char>& out, const unsigned char* in, size_t insize,
                  const LodePNGCompressSettings& settings)
{
  unsigned char* buffer = 0;
  size_t buffersize = 0;
  unsigned error = zlib_compress(&buffer, &buffersize, in, insize, &settings);
  if(buffer)
  {
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned compress(std::vector<unsigned char>& out, const std::vector<unsigned char>& in,
                  const LodePNGCompressSettings& settings)
{
  return compress(out, in.empty() ? 0 : &in[0], in.size(), settings);
}
#endif //LODEPNG_COMPILE_ENCODER
#endif //LODEPNG_COMPILE_ZLIB


#ifdef LODEPNG_COMPILE_PNG

State::State()
{
  lodepng_state_init(this);
}

State::State(const State& other)
{
  lodepng_state_init(this);
  lodepng_state_copy(this, &other);
}

State::~State()
{
  lodepng_state_cleanup(this);
}

State& State::operator=(const State& other)
{
  lodepng_state_copy(this, &other);
  return *this;
}

#ifdef LODEPNG_COMPILE_DECODER

unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h, const unsigned char* in,
                size_t insize, LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned char* buffer;
  unsigned error = lodepng_decode_memory(&buffer, &w, &h, in, insize, colortype, bitdepth);
  if(buffer && !error)
  {
    State state;
    state.info_raw.colortype = colortype;
    state.info_raw.bitdepth = bitdepth;
    size_t buffersize = lodepng_get_raw_size(w, h, &state.info_raw);
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const std::vector<unsigned char>& in, LodePNGColorType colortype, unsigned bitdepth)
{
  return decode(out, w, h, in.empty() ? 0 : &in[0], (unsigned)in.size(), colortype, bitdepth);
}

unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                State& state,
                const unsigned char* in, size_t insize)
{
  unsigned char* buffer;
  unsigned error = lodepng_decode(&buffer, &w, &h, &state, in, insize);
  if(buffer && !error)
  {
    size_t buffersize = lodepng_get_raw_size(w, h, &state.info_raw);
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                State& state,
                const std::vector<unsigned char>& in)
{
  return decode(out, w, h, state, in.empty() ? 0 : &in[0], in.size());
}

#ifdef LODEPNG_COMPILE_DISK
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h, const std::string& filename,
                LodePNGColorType colortype, unsigned bitdepth)
{
  std::vector<unsigned char> buffer;
  load_file(buffer, filename);
  return decode(out, w, h, buffer, colortype, bitdepth);
}
#endif //LODEPNG_COMPILE_DECODER
#endif //LODEPNG_COMPILE_DISK

#ifdef LODEPNG_COMPILE_ENCODER
unsigned encode(std::vector<unsigned char>& out, const unsigned char* in, unsigned w, unsigned h,
                LodePNGColorType colortype, unsigned bitdepth)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error = lodepng_encode_memory(&buffer, &buffersize, in, w, h, colortype, bitdepth);
  if(buffer)
  {
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned encode(std::vector<unsigned char>& out,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                LodePNGColorType colortype, unsigned bitdepth)
{
  if(lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size()) return 84;
  return encode(out, in.empty() ? 0 : &in[0], w, h, colortype, bitdepth);
}

unsigned encode(std::vector<unsigned char>& out,
                const unsigned char* in, unsigned w, unsigned h,
                State& state)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error = lodepng_encode(&buffer, &buffersize, in, w, h, &state);
  if(buffer)
  {
    out.insert(out.end(), &buffer[0], &buffer[buffersize]);
    lodepng_free(buffer);
  }
  return error;
}

unsigned encode(std::vector<unsigned char>& out,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                State& state)
{
  if(lodepng_get_raw_size(w, h, &state.info_raw) > in.size()) return 84;
  return encode(out, in.empty() ? 0 : &in[0], w, h, state);
}

#ifdef LODEPNG_COMPILE_DISK
unsigned encode(const std::string& filename,
                const unsigned char* in, unsigned w, unsigned h,
                LodePNGColorType colortype, unsigned bitdepth)
{
  std::vector<unsigned char> buffer;
  unsigned error = encode(buffer, in, w, h, colortype, bitdepth);
  if(!error) save_file(buffer, filename);
  return error;
}

unsigned encode(const std::string& filename,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                LodePNGColorType colortype, unsigned bitdepth)
{
  if(lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size()) return 84;
  return encode(filename, in.empty() ? 0 : &in[0], w, h, colortype, bitdepth);
}
#endif //LODEPNG_COMPILE_DISK
#endif //LODEPNG_COMPILE_ENCODER
#endif //LODEPNG_COMPILE_PNG
} //namespace lodepng
#endif /*LODEPNG_COMPILE_CPP*/



 

const int MAX_KEYS = 256;


enum eProgramState {
	E_PS_MENU,
	E_PS_IN_GAME,
	E_PS_SIZE
};

enum eProgramAction {
	E_PA_QUIT,
	E_PA_TOGGLE_FULLSCREEN,
	E_PA_REFRESH,
	E_PA_SIZE
};

enum E_TEX_TYPE {
    E_TEX_TYPE_NOISE
};

enum E_RENDER_METHODS {
    E_RENDER_NONE,
    E_RENDER_VOL,
    E_RENDER_LENGTH
};

enum E_STATES {
    E_STATE_INIT_LAUNCH,
    E_STATE_INIT_BEG,
    E_STATE_INIT_END,
    E_STATE_CREATESIMPLEXNOISE_LAUNCH,
    E_STATE_CREATESIMPLEXNOISE_BEG,
    E_STATE_CREATESIMPLEXNOISE_END,
    E_STATE_NEIGHBORSREADY_LAUNCH,
    E_STATE_NEIGHBORSREADY_BEG,
    E_STATE_NEIGHBORSREADY_END,
    E_STATE_COPYTOTEXTURE_LAUNCH,
    E_STATE_COPYTOTEXTURE_BEG,
    E_STATE_COPYTOTEXTURE_END,
    E_STATE_GENERATEVOLUME_LAUNCH,
    E_STATE_GENERATEVOLUME_BEG,
    E_STATE_GENERATEVOLUME_END,
    E_STATE_WAIT,
    E_STATE_LENGTH

};

enum E_MOUSE_STATE {
    E_MOUSE_STATE_MOVE,
    E_MOUSE_STATE_OBJECTS,
    E_MOUSE_STATE_BRUSH,
    E_MOUSE_STATE_LENGTH
};

enum E_BRUSH {
    E_BRUSH_ADD,
    E_BRUSH_SUB,
    E_BRUSH_MOVE,
    E_BRUSH_LENGTH
};



////////////////////////////////////////////////////

//    DONT FORGET SEMICOLONS!

////////////////////////////////////////////////////



enum E_GRASS_STATE {
    E_GRASS_STATE_OFF,
    E_GRASS_STATE_ON,
    E_GRASS_STATE_ANIM,
    E_GRASS_STATE_LENGTH
};

enum E_FILL_STATE {
    E_FILL_STATE_EMPTY,
    E_FILL_STATE_PARTIAL,
    E_FILL_STATE_FULL,
};

enum E_OBJ {
    E_OBJ_NONE,
    E_OBJ_STONE,
    E_OBJ_DIRT,
    E_OBJ_GRASS,
    E_OBJ_LIGHT,
    E_OBJ_FOG,
    E_OBJ_LENGTH
};


//#define DEBUG_MODE 

//const static int MAX_THREADS = 8; 
int traceLevel = 0; int popCount = 0;

std::string i__s(int i) {
	return std::to_string(i);
}
std::string f__s(float f) {
	return std::to_string(f);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	return split(s, delim, elems);
}


inline float clamp(float val) {
	float retval = val;
	if (retval < 0.0f) retval = 0.0f;
	if (retval > 1.0f) retval = 1.0f;
	return retval;
}

inline float fGenRand() {
	return ((float)(rand()%100000))/100000.0f;
}

inline int iGenRand(int val) {
	return rand()%(val+1) - val/2;
}

std::string intToString(int i) {
	std::string s;
	std::stringstream out;
	out << i;
	s = out.str();
	return s;
}
std::string floatToString(float f) {
	std::string s;
	std::stringstream out;
	out << f;
	s = out.str();
	return s;
}



void doTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	int i;
	
	for (i = 0; i < traceLevel; i++) {
		std::cout << "|  ";
	}
	
	std::cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n" << std::flush;
	
}
void doTraceVec(std::string traceVal0, FIVector4 *fv) {
	doTrace(traceVal0, " ", f__s(fv->getFX()), " ", f__s(fv->getFY()), " ", f__s(fv->getFZ())  );
}

void pushTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	doTrace(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
	traceLevel++;
	popCount=0;
}
void popTrace() {
	traceLevel--;
	popCount++;
	if (popCount >= 2) {
		doTrace("END");
	}
}

float distance(Vector3* v1, Vector3* v2) {
	float val1 = v1->e[0] - v2->e[0];
	float val2 = v1->e[1] - v2->e[1];
	float val3 = v1->e[2] - v2->e[2];
	
	return sqrt(
		val1*val1 + val2*val2 + val3*val3
	);
	
	//return length(v1-v2);
} 

const static int MAX_OG_RAD = 20;
const static int MAX_OG_DIAM = MAX_OG_RAD*2+1;
const static int MAX_OG_SIZE = MAX_OG_DIAM * MAX_OG_DIAM * MAX_OG_DIAM;
const static int OG_NULL_VAL = 9999;

class OffsetGrid {
private:
	
public:
	iVector3 offsetGrid[MAX_OG_RAD+1][MAX_OG_SIZE];
	iVector3 offsetGridCum[(MAX_OG_RAD+1)*(MAX_OG_SIZE)];

	int offsetGridCounters[MAX_OG_RAD+1];
	int offsetGridCumCounters[MAX_OG_RAD+1];

	
	
	OffsetGrid() {
		int i;
		int j;
		int k;
		int curCount;
		
		for (j = 0; j < MAX_OG_RAD+1; j++) {
			offsetGridCounters[j] = 0;
			for (i = 0; i < MAX_OG_SIZE; i++) {
				offsetGrid[j][i].x = OG_NULL_VAL;
				offsetGrid[j][i].y = OG_NULL_VAL;
				offsetGrid[j][i].z = OG_NULL_VAL;
			}
		}
		
		
		int xdis;
		int ydis;
		int zdis;
		
		int max1;
		int max2;
		int ind;

		
		
		for (i = 0; i < MAX_OG_DIAM; i++) {
			for (j = 0; j < MAX_OG_DIAM; j++) {
				for (k = 0; k < MAX_OG_DIAM; k++) {
					xdis = i-MAX_OG_RAD;
					ydis = j-MAX_OG_RAD;
					zdis = k-MAX_OG_RAD;
					
					max1 = max(abs(xdis),abs(ydis));
					max2 = max(max1,abs(zdis));
					
					ind = offsetGridCounters[max2];
					
					offsetGrid[max2][ind].x = xdis;
					offsetGrid[max2][ind].y = ydis;
					offsetGrid[max2][ind].z = zdis;
					

					offsetGridCounters[max2]++;
					
					
					
				}
			}
		}

		curCount = 0;
		

		for (j = 0; j <= MAX_OG_RAD; j++) {
			for (i = 0; i < offsetGridCounters[j]; i++) {
				offsetGridCum[curCount].x = offsetGrid[j][i].x;
				offsetGridCum[curCount].y = offsetGrid[j][i].y;
				offsetGridCum[curCount].z = offsetGrid[j][i].z;

				curCount++;
			}
			offsetGridCumCounters[j] = curCount;

		}

	}
};



class Shader {
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
public:
	
	Shader() {
		pushTrace("Shader()");
		popTrace();
	}

	Shader(const char *shaderFile) {
		pushTrace("Shader(", shaderFile, ")");
		init(shaderFile);
		popTrace();
	}


	static char* textFileRead(const char *fileName) {
		pushTrace("textFileRead(",  fileName, ")");
		
		char* text = "";
		bool failed = true;
	    
		if (fileName != NULL) {
	        FILE *file = fopen(fileName, "rt");
	        
			if (file != NULL) {
	            fseek(file, 0, SEEK_END);
	            int count = ftell(file);
	            rewind(file);
	            
				if (count > 0) {
					text = (char*)malloc(sizeof(char) * (count + 1));
					count = fread(text, sizeof(char), count, file);
					text[count] = '\0';
					failed = false;
				}
				fclose(file);
			}
		}

		if (failed) {
			doTrace("FAILED TO READ FILE: ", fileName);
		}
		else {
			doTrace("READ FILE: ", fileName);
		}
		
		popTrace();
		
		return text;
	}

	static void validateShader(GLuint shader, const char* file = 0) {
		pushTrace("validateShader(", file, ")");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			doTrace("Shader " , i__s(shader) , " (" , (file?file:"") , ") compile error: " , buffer);
		}
		popTrace();
	}

	static int validateProgram(GLuint program) {
		pushTrace("validateProgram()");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		memset(buffer, 0, BUFFER_SIZE);
		glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			doTrace( "Program " , i__s(program) , " link error: " , buffer);
			popTrace();
			return 0;
		}
	    
		glValidateProgram(program);
		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE) {
			doTrace( "Error validating shader " , i__s(program));
			popTrace();
			return 0;
		}
		
		popTrace();
		return 1;
		
	}
	

	void init(const char *shaderFile) {
		pushTrace("init(", shaderFile, ")");
		shader_vp = glCreateShader(GL_VERTEX_SHADER);
		shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
	    
	    
		const char* allText = textFileRead(shaderFile);


		if (allText == NULL) {
			doTrace( "Either vertex shader or fragment shader file not found." );
	    }
	    else {
	    	string allTextString(allText);

			vector<string> allTextStringSplit = split(allTextString, '$');

			if (allTextStringSplit.size() == 3) {

				string vertStr = allTextStringSplit[0] + allTextStringSplit[1];
				string fragStr = allTextStringSplit[0] + allTextStringSplit[2];

				const GLchar* vertCS = new char[vertStr.length() + 1];
				const GLchar* fragCS = new char[fragStr.length() + 1];

				std::strcpy((GLchar*)vertCS,vertStr.c_str());
				std::strcpy((GLchar*)fragCS,fragStr.c_str());


		    	glShaderSource(shader_vp, 1, &(vertCS), 0);
				glShaderSource(shader_fp, 1, &(fragCS), 0);
			    
				glCompileShader(shader_vp);
				validateShader(shader_vp, shaderFile);
				glCompileShader(shader_fp);
				validateShader(shader_fp, shaderFile);
			    
				shader_id = glCreateProgram();
				glAttachShader(shader_id, shader_fp);
				glAttachShader(shader_id, shader_vp);
				glLinkProgram(shader_id);
				validateProgram(shader_id);

				delete [] vertCS;
				delete [] fragCS;

			}
			else {
				doTrace( "Error: " , shaderFile , "does not contain proper amount of splits ($)\n" );
			}
			
			
	    }
		
		
		popTrace();
		
	}

	~Shader() {
		pushTrace("~Shader()");
		glDetachShader(shader_id, shader_fp);
		glDetachShader(shader_id, shader_vp);
	    
		glDeleteShader(shader_fp);
		glDeleteShader(shader_vp);
		glDeleteProgram(shader_id);
		popTrace();
	}

	unsigned int id() {
		pushTrace("id()");
		return shader_id;
		popTrace();
	}

	void bind() {
		//pushTrace("bind()");
		glUseProgram(shader_id);
		//popTrace();
	}

	void unbind() {
		//pushTrace("unbind()");
		glUseProgram(0);
		//popTrace();
	}

	void setTexture(const GLchar* name, int texUnit, int texID) {
		GLint baseImageLoc = glGetUniformLocation(shader_id, name);

		glUniform1i(baseImageLoc, texUnit); //Texture unit 0 is for base images.
		
		//When rendering an objectwith this program.
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	
	void setVec(const GLchar* name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name);
		
		switch (vecSize) {
			case 0:
				doTrace( "Error: vecSize of 0 in setVec" );
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setVecString(string name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name.c_str());
		
		switch (vecSize) {
			case 0:
				doTrace( "Error: vecSize of 0 in setVec" );
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setShaderFloat(string paramName, float x) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1f(loc,x);
	}
	void setShaderVec2(string paramName, float x, float y) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform2f(loc,x,y);
	}
	void setShaderVec3(string paramName, float x, float y, float z) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3f(loc,x,y,z);
	}
	void setShaderVec4(string paramName, float x, float y, float z, float w) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4f(loc,x,y,z,w);
	}

	void setShaderInt(string paramName, int multitexNumber) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1i(loc,multitexNumber);
	}



	void setShaderfVec2(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform2f(loc,f->getFX(),f->getFY());
	}
	void setShaderfVec3(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3f(loc,f->getFX(),f->getFY(),f->getFZ());
	}
	void setShaderfVec4(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4f(loc,f->getFX(),f->getFY(),f->getFZ(),f->getFW());
	}

	

};





 
class Timer
{
public:
    Timer(){
        #ifdef WIN32
            QueryPerformanceFrequency(&frequency);
            startCount.QuadPart = 0;
            endCount.QuadPart = 0;
        #else
            startCount.tv_sec = startCount.tv_usec = 0;
            endCount.tv_sec = endCount.tv_usec = 0;
        #endif

            stopped = 0;
            startTimeInMicroSec = 0;
            endTimeInMicroSec = 0;
        }
    ~Timer() {

    }

    void   start() {
        stopped = 0; // reset stop flag
        #ifdef WIN32
        QueryPerformanceCounter(&startCount);
        #else
        gettimeofday(&startCount, NULL);
        #endif
    }
    void   stop() {
        stopped = 1; // set timer stopped flag

        #ifdef WIN32
        QueryPerformanceCounter(&endCount);
        #else
        gettimeofday(&endCount, NULL);
        #endif
    }
    double getElapsedTime() {
        return this->getElapsedTimeInSec();
    }
    double getElapsedTimeInSec() {
        return this->getElapsedTimeInMicroSec() * 0.000001;
    }

    double getElapsedTimeInMilliSec() {
        return this->getElapsedTimeInMicroSec() * 0.001;
    }

    double getElapsedTimeInMicroSec() {
        #ifdef WIN32
        if(!stopped) {
            QueryPerformanceCounter(&endCount);
        }

        startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
        endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
        #else
        if(!stopped) {
            gettimeofday(&endCount, NULL);
        }

        startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
        endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
        #endif

        return endTimeInMicroSec - startTimeInMicroSec;
    }



protected:


private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
}; 


// Multi-octave Simplex noise - multiple noise values are combined.
float simplexNoise( const float, const float, const float, const float, const float );
float simplexNoise( const float, const float, const float, const float, const float, const float );
float simplexNoise( const float, const float, const float, const float, const float, const float, const float );


// Multi-octave Scaled Simplex noise - the result will be between the two parameters passed.
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float );
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float, const float );
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float, const float, const float );

// Scaled Raw Simplex noise - the result will be between the two parameters passed.
float simplexScaledRawNoise( const float, const float, const float, const float );
float simplexScaledRawNoise( const float, const float, const float, const float, const float );
float simplexScaledRawNoise( const float, const float, const float, const float, const float, const float );


// Raw Simplex noise - a single noise value.
float simplexRawNoise( const float, const float );
float simplexRawNoise( const float, const float, const float );
float simplexRawNoise( const float, const float, const float, const float );


int fastfloor( const float x ) { return x > 0 ? (int) x : (int) x - 1; }
float dot( const int* g, const float x, const float y ) { return g[0]*x + g[1]*y; }
float dot( const int* g, const float x, const float y, const float z ) { return g[0]*x + g[1]*y + g[2]*z; }
float dot( const int* g, const float x, const float y, const float z, const float w ) { return g[0]*x + g[1]*y + g[2]*z + g[3]*w; }




// The gradients are the midpoints of the vertices of a cube.
static const int grad3[12][3] = {
	{1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
	{1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
	{0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};


// The gradients are the midpoints of the vertices of a hypercube.
static const int grad4[32][4]= {
	{0,1,1,1},  {0,1,1,-1},  {0,1,-1,1},  {0,1,-1,-1},
	{0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
	{1,0,1,1},  {1,0,1,-1},  {1,0,-1,1},  {1,0,-1,-1},
	{-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
	{1,1,0,1},  {1,1,0,-1},  {1,-1,0,1},  {1,-1,0,-1},
	{-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
	{1,1,1,0},  {1,1,-1,0},  {1,-1,1,0},  {1,-1,-1,0},
	{-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
};


// Permutation table.  The same list is repeated twice.
static const int perm[512] = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


// A lookup table to traverse the simplex around a given point in 4D.
static const int simplex[64][4] = {
	{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
	{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
	{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
	{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};


// 2D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 3D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y, const float z ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency, z * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 4D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y, const float z, const float w ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency, z * frequency, w * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}



// 2D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y ) {
	return simplexNoise(octaves, persistence, scale, x, y) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}


// 3D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
	return simplexNoise(octaves, persistence, scale, x, y, z) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}

// 4D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z, const float w ) {
	return simplexNoise(octaves, persistence, scale, x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}



// 2D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y ) {
	return simplexRawNoise(x, y) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}


// 3D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y, const float z ) {
	return simplexRawNoise(x, y, z) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}

// 4D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y, const float z, const float w ) {
	return simplexRawNoise(x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}



// 2D raw Simplex noise
float simplexRawNoise( const float x, const float y ) {
	// Noise contributions from the three corners
	float n0, n1, n2;

	// Skew the input space to determine which simplex cell we're in
	float F2 = 0.5 * (sqrtf(3.0) - 1.0);
	// Hairy factor for 2D
	float s = (x + y) * F2;
	int i = fastfloor( x + s );
	int j = fastfloor( y + s );

	float G2 = (3.0 - sqrtf(3.0)) / 6.0;
	float t = (i + j) * G2;
	// Unskew the cell origin back to (x,y) space
	float X0 = i-t;
	float Y0 = j-t;
	// The x,y distances from the cell origin
	float x0 = x-X0;
	float y0 = y-Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
	if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0 + 2.0 * G2;

	// Work out the hashed gradient indices of the three simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = perm[ii+perm[jj]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1]] % 12;
	int gi2 = perm[ii+1+perm[jj+1]] % 12;

	// Calculate the contribution from the three corners
	float t0 = 0.5 - x0*x0-y0*y0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
	}

	float t1 = 0.5 - x1*x1-y1*y1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
	}

	float t2 = 0.5 - x2*x2-y2*y2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}


// 3D raw Simplex noise
float simplexRawNoise( const float x, const float y, const float z ) {
	float n0, n1, n2, n3; // Noise contributions from the four corners

	// Skew the input space to determine which simplex cell we're in
	float F3 = 1.0/3.0;
	float s = (x+y+z)*F3; // Very nice and simple skew factor for 3D
	int i = fastfloor(x+s);
	int j = fastfloor(y+s);
	int k = fastfloor(z+s);

	float G3 = 1.0/6.0; // Very nice and simple unskew factor, too
	float t = (i+j+k)*G3;
	float X0 = i-t; // Unskew the cell origin back to (x,y,z) space
	float Y0 = j-t;
	float Z0 = k-t;
	float x0 = x-X0; // The x,y,z distances from the cell origin
	float y0 = y-Y0;
	float z0 = z-Z0;

	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

	if(x0>=y0) {
	if(y0>=z0)
	{ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
	else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
	else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
	}
	else { // x0<y0
	if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
	else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
	else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
	}

	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.
	float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0*G3; // Offsets for third corner in (x,y,z) coords
	float y2 = y0 - j2 + 2.0*G3;
	float z2 = z0 - k2 + 2.0*G3;
	float x3 = x0 - 1.0 + 3.0*G3; // Offsets for last corner in (x,y,z) coords
	float y3 = y0 - 1.0 + 3.0*G3;
	float z3 = z0 - 1.0 + 3.0*G3;

	// Work out the hashed gradient indices of the four simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int gi0 = perm[ii+perm[jj+perm[kk]]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1]]] % 12;
	int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2]]] % 12;
	int gi3 = perm[ii+1+perm[jj+1+perm[kk+1]]] % 12;

	// Calculate the contribution from the four corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
	}

	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
	}

	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
	}

	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
	if(t3<0) n3 = 0.0;
	else {
	t3 *= t3;
	n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0*(n0 + n1 + n2 + n3);
}


// 4D raw Simplex noise
float simplexRawNoise( const float x, const float y, const float z, const float w ) {
	// The skewing and unskewing factors are hairy again for the 4D case
	float F4 = (sqrtf(5.0)-1.0)/4.0;
	float G4 = (5.0-sqrtf(5.0))/20.0;
	float n0, n1, n2, n3, n4; // Noise contributions from the five corners

	// Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
	float s = (x + y + z + w) * F4; // Factor for 4D skewing
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);
	int k = fastfloor(z + s);
	int l = fastfloor(w + s);
	float t = (i + j + k + l) * G4; // Factor for 4D unskewing
	float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
	float Y0 = j - t;
	float Z0 = k - t;
	float W0 = l - t;

	float x0 = x - X0; // The x,y,z,w distances from the cell origin
	float y0 = y - Y0;
	float z0 = z - Z0;
	float w0 = w - W0;

	// For the 4D case, the simplex is a 4D shape I won't even try to describe.
	// To find out which of the 24 possible simplices we're in, we need to
	// determine the magnitude ordering of x0, y0, z0 and w0.
	// The method below is a good way of finding the ordering of x,y,z,w and
	// then find the correct traversal order for the simplex we're in.
	// First, six pair-wise comparisons are performed between each possible pair
	// of the four coordinates, and the results are used to add up binary bits
	// for an integer index.
	int c1 = (x0 > y0) ? 32 : 0;
	int c2 = (x0 > z0) ? 16 : 0;
	int c3 = (y0 > z0) ? 8 : 0;
	int c4 = (x0 > w0) ? 4 : 0;
	int c5 = (y0 > w0) ? 2 : 0;
	int c6 = (z0 > w0) ? 1 : 0;
	int c = c1 + c2 + c3 + c4 + c5 + c6;

	int i1, j1, k1, l1; // The integer offsets for the second simplex corner
	int i2, j2, k2, l2; // The integer offsets for the third simplex corner
	int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

	// simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
	// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
	// impossible. Only the 24 indices which have non-zero entries make any sense.
	// We use a thresholding to set the coordinates in turn from the largest magnitude.
	// The number 3 in the "simplex" array is at the position of the largest coordinate.
	i1 = simplex[c][0]>=3 ? 1 : 0;
	j1 = simplex[c][1]>=3 ? 1 : 0;
	k1 = simplex[c][2]>=3 ? 1 : 0;
	l1 = simplex[c][3]>=3 ? 1 : 0;
	// The number 2 in the "simplex" array is at the second largest coordinate.
	i2 = simplex[c][0]>=2 ? 1 : 0;
	j2 = simplex[c][1]>=2 ? 1 : 0;
	k2 = simplex[c][2]>=2 ? 1 : 0;
	l2 = simplex[c][3]>=2 ? 1 : 0;
	// The number 1 in the "simplex" array is at the second smallest coordinate.
	i3 = simplex[c][0]>=1 ? 1 : 0;
	j3 = simplex[c][1]>=1 ? 1 : 0;
	k3 = simplex[c][2]>=1 ? 1 : 0;
	l3 = simplex[c][3]>=1 ? 1 : 0;
	// The fifth corner has all coordinate offsets = 1, so no need to look that up.

	float x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
	float y1 = y0 - j1 + G4;
	float z1 = z0 - k1 + G4;
	float w1 = w0 - l1 + G4;
	float x2 = x0 - i2 + 2.0*G4; // Offsets for third corner in (x,y,z,w) coords
	float y2 = y0 - j2 + 2.0*G4;
	float z2 = z0 - k2 + 2.0*G4;
	float w2 = w0 - l2 + 2.0*G4;
	float x3 = x0 - i3 + 3.0*G4; // Offsets for fourth corner in (x,y,z,w) coords
	float y3 = y0 - j3 + 3.0*G4;
	float z3 = z0 - k3 + 3.0*G4;
	float w3 = w0 - l3 + 3.0*G4;
	float x4 = x0 - 1.0 + 4.0*G4; // Offsets for last corner in (x,y,z,w) coords
	float y4 = y0 - 1.0 + 4.0*G4;
	float z4 = z0 - 1.0 + 4.0*G4;
	float w4 = w0 - 1.0 + 4.0*G4;

	// Work out the hashed gradient indices of the five simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int ll = l & 255;
	int gi0 = perm[ii+perm[jj+perm[kk+perm[ll]]]] % 32;
	int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1+perm[ll+l1]]]] % 32;
	int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2+perm[ll+l2]]]] % 32;
	int gi3 = perm[ii+i3+perm[jj+j3+perm[kk+k3+perm[ll+l3]]]] % 32;
	int gi4 = perm[ii+1+perm[jj+1+perm[kk+1+perm[ll+1]]]] % 32;

	// Calculate the contribution from the five corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0 - w0*w0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
	}

	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1 - w1*w1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
	}

	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2 - w2*w2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
	}

	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3 - w3*w3;
	if(t3<0) n3 = 0.0;
	else {
	t3 *= t3;
	n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
	}

	float t4 = 0.6 - x4*x4 - y4*y4 - z4*z4 - w4*w4;
	if(t4<0) n4 = 0.0;
	else {
	t4 *= t4;
	n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
	}

	// Sum up and scale the result to cover the range [-1,1]
	return 27.0 * (n0 + n1 + n2 + n3 + n4);
}
 


//Converts a four-character array to an integer, using little-endian form
int toInt(const char* bytes) {
	return (int)(((unsigned char)bytes[3] << 24) |
				 ((unsigned char)bytes[2] << 16) |
				 ((unsigned char)bytes[1] << 8) |
				 (unsigned char)bytes[0]);
}

//Converts a two-character array to a short, using little-endian form
short toShort(const char* bytes) {
	return (short)(((unsigned char)bytes[1] << 8) |
				   (unsigned char)bytes[0]);
}

//Reads the next four bytes as an integer, using little-endian form
int readInt(ifstream &input) {
	char buffer[4];
	input.read(buffer, 4);
	return toInt(buffer);
}

//Reads the next two bytes as a short, using little-endian form
short readShort(ifstream &input) {
	char buffer[2];
	input.read(buffer, 2);
	return toShort(buffer);
}



//Just like auto_ptr, but for arrays
template<class T>
class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}
		
		auto_array(const auto_array<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}
		
		T* get() const {
			return array;
		}
		
		T &operator*() const {
			return *array;
		}
		
		void operator=(const auto_array<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		T* operator->() const {
			return array;
		}
		
		T* release() {
			isReleased = true;
			return array;
		}
		
		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}
		
		T* operator+(int i) {
			return array + i;
		}
		
		T &operator[](int i) {
			return array[i];
		}
};

//Represents an image
class Image {
public:
		
	/* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
	 * color of each pixel in image.  Color components range from 0 to 255.
	 * The array starts the bottom-left pixel, then moves right to the end
	 * of the row, then moves up to the next column, and so on.  This is the
	 * format in which OpenGL likes images.
	 */
	char* pixels;
	int width;
	int height;

	Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {
		
	}

	~Image() {
		delete[] pixels;
	}

};


Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);
	
	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	switch(headerSize) {
		case 40:
			//V3
			width = readInt(input);
			height = readInt(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			assert(readShort(input) == 0 || !"Image is compressed");
			break;
		case 12:
			//OS/2 V1
			width = readShort(input);
			height = readShort(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			break;
		case 64:
			//OS/2 V2
			assert(!"Can't load OS/2 V2 bitmaps");
			break;
		case 108:
			//Windows V4
			assert(!"Can't load Windows V4 bitmaps");
			break;
		case 124:
			//Windows V5
			assert(!"Can't load Windows V5 bitmaps");
			break;
		default:
			assert(!"Unknown bitmap format");
	}
	
	//Read the data
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);
	
	//Get the data into the right format
	auto_array<char> pixels2(new char[width * height * 3]);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}
	
	input.close();
	return new Image(pixels2.release(), width, height);
}

GLuint loadTexture(Image* image) {
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return tid;
}
 
class FBOWrapper
{
public:

	uint color_tex;
	uint depth_rb;
	//uint color_buf;
	//uint depth_buf;
	uint slot;
	int width;
	int height;
	int bytesPerChannel;
	bool hasDepth;


	GLint internalFormat;

	unsigned char *pixelsChar;
	float *pixelsFloat;
	bool isFloat;

    FBOWrapper() {}
    ~FBOWrapper() {}
    int init(int _width, int _height, int _bytesPerChannel, int _slot, bool _hasDepth) {
		width = _width;
		height = _height;
		bytesPerChannel = _bytesPerChannel;
		hasDepth = _hasDepth;

		int w = width;
		int h = height;

		isFloat = false;

		pixelsChar = NULL;
		pixelsFloat = NULL;


		slot = GL_COLOR_ATTACHMENT0_EXT;

		switch (_slot) {
			case 0:
				slot = GL_COLOR_ATTACHMENT0_EXT;
			break;
			case 1:
				slot = GL_COLOR_ATTACHMENT1_EXT;
			break;
			case 2:
				slot = GL_COLOR_ATTACHMENT2_EXT;
			break;
			case 3:
				slot = GL_COLOR_ATTACHMENT3_EXT;
			break;
			case 4:
				slot = GL_COLOR_ATTACHMENT4_EXT;
			break;
			case 5:
				slot = GL_COLOR_ATTACHMENT5_EXT;
			break;
			case 6:
				slot = GL_COLOR_ATTACHMENT6_EXT;
			break;
			case 7:
				slot = GL_COLOR_ATTACHMENT7_EXT;
			break;
		}

	    

		glGenTextures(1, &color_tex);
		glBindTexture(GL_TEXTURE_2D, color_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	    switch (bytesPerChannel) {
	    	case 1:
	    		internalFormat = GL_RGBA8;
	    		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	    		
	    	break;
	    	case 2:
	    		internalFormat = GL_RGBA16;
	            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);

	    	break;
	    	case 4:
	    		internalFormat = GL_RGBA32F;
	            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	            isFloat = true;

	    	break;
	    }

	    TOT_GPU_MEM_USAGE += ((float)(w*h*bytesPerChannel*4))/(1024.0f*1024.0f);


	    //

	    if (hasDepth) {
	    	glGenRenderbuffersEXT(1, &depth_rb);
	    	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	    	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
	    	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	    }

	    
	    //

		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D, color_tex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	    return 1;

	}



	void getPixelAtF(FIVector4* fv, int x, int y) {

		if (isFloat) {


			if ( (pixelsFloat == NULL) ) {
				getPixels();
			}

			//a
			//b
			//g
			//r

			fv->setFX(pixelsFloat[ (x + y*width)*4 + 0 ]);
			fv->setFY(pixelsFloat[ (x + y*width)*4 + 1 ]);
			fv->setFZ(pixelsFloat[ (x + y*width)*4 + 2 ]);
			fv->setFW(pixelsFloat[ (x + y*width)*4 + 3 ]);

		}
		else {
			doTrace("Attempted to call getPixelAtF on char buffer.");
		}

	}

	unsigned char getPixelAtC(int x, int y, int channel) {

		if (isFloat) {
			if ( (pixelsFloat == NULL) ) {
				getPixels();
			}

			return pixelsChar[ (x + y*width)*4 + channel ];

		}
		else {
			doTrace("Attempted to call getPixelAtC on float buffer.");
		}

	}


	void getPixels() {

		glBindTexture(GL_TEXTURE_2D, color_tex);
		GLint numBytes = 0;
		
		//GLint intForm;
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &intForm); // get internal format type of GL texture


		switch(internalFormat) // determine what type GL texture has...
		{
			case GL_RGB:
				numBytes = width * height * 3;
			break;
			case GL_RGBA:
				numBytes = width * height * 4;
			break;
			case GL_RGBA8:
				numBytes = width * height * 4;
			break;
			//case GL_RGBA16:
			//
			//break;
			case GL_RGBA32F:
				numBytes = width * height * 4 * 4;
			break;

			default:
				doTrace("Unsupported Format Type");
				return;
			break;
		}

		if (numBytes) {

			if (isFloat) {

				if (pixelsFloat == NULL) {
					pixelsFloat = (float*)malloc(numBytes);
				}
				
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixelsFloat);
			}
			else {

				if (pixelsChar == NULL) {
					pixelsChar = (unsigned char*)malloc(numBytes);
				}
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsChar);
			}

			
		}

		glBindTexture(GL_TEXTURE_2D, 0);


	}


};


class FBOSet
{
public:
	int numBufs;
	int width;
	int height;
	int bytesPerChannel;

	GLuint mFBO;

	FBOWrapper* fbos;

	bool hasDepth;

    FBOSet() {}
    ~FBOSet() {}

    FBOWrapper* getFBOWrapper(int offset) {
    	return &(fbos[offset]);
    }

    void init(int _numBufs, int _width, int _height, int _bytesPerChannel, bool _hasDepth) {
		int i;

		hasDepth = _hasDepth;

		numBufs = _numBufs;
		height = _height;
		width = _width;
		bytesPerChannel = _bytesPerChannel;

		fbos = new FBOWrapper[numBufs];


		glGenFramebuffersEXT(1, &mFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		for (i = 0; i < numBufs; i++) {
			fbos[i].init(width, height, bytesPerChannel, i, hasDepth);
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
    void bind(int doClear) {

		//setWH(width, height);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT};
		glDrawBuffers(numBufs, buffers);

		if (doClear) {
	    	glClearColor(0.0,0.0,0.0,0.0);
	    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    }

	}
	/*
    void setWH(int w, int h) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
	}
	*/
    void copyFromMem(int ind, uint* dat) {

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

	    glBindTexture(GL_TEXTURE_2D,fbos[ind].color_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, fbos[ind].slot, GL_TEXTURE_2D, fbos[ind].color_tex, 0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
};



/*
//RGBA8 2D texture, 24 bit depth texture, 256x256
   glGenTextures(1, &color_tex);
   glBindTexture(GL_TEXTURE_2D, color_tex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //NULL means reserve texture memory, but texels are undefined
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
   //-------------------------
   glGenFramebuffersEXT(1, &fb);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
   //Attach 2D texture to this FBO
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
   //-------------------------
   glGenRenderbuffersEXT(1, &depth_rb);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);
   //-------------------------
   //Attach depth buffer to FBO
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
   //-------------------------
   //Does the GPU support current FBO configuration?
   GLenum status;
   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   switch(status)
   {
      case GL_FRAMEBUFFER_COMPLETE_EXT:
      cout<<"good";
   default:
      HANDLE_THE_ERROR;
   }
   //-------------------------
   //and now you can render to GL_TEXTURE_2D
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //-------------------------
   glViewport(0, 0, 256, 256);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 256.0, 0.0, 256.0, -1.0, 1.0); 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //-------------------------
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   //-------------------------
   //**************************
   //RenderATriangle, {0.0, 0.0}, {256.0, 0.0}, {256.0, 256.0}
   //Read http://www.opengl.org/wiki/VBO_-_just_examples
   RenderATriangle();
   //-------------------------
   GLubyte pixels[4*4*4];
   glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   //pixels 0, 1, 2 should be white
   //pixel 4 should be black
   //----------------
   //Bind 0, which means render to back buffer
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
*/

 

class WebSocketRequestHandler;
class WebSocketServer;
class RequestHandlerFactory;
class Singleton;
class GamePage;
class GameWorld;
class PooledResource;
 
// f00290_wsrequesthandler.e
//

#ifndef LZZ_f00290_wsrequesthandler_e
#define LZZ_f00290_wsrequesthandler_e
#define LZZ_INLINE inline
class WebSocketRequestHandler : public HTTPRequestHandler
{
public:
  WebSocketServer * ws_ptr;
  WebSocketRequestHandler (WebSocketServer * _ws_ptr);
  ~ WebSocketRequestHandler ();
  void handleRequest (HTTPServerRequest & request, HTTPServerResponse & response);
};
#undef LZZ_INLINE
#endif
// f00291_rhfactory.e
//

#ifndef LZZ_f00291_rhfactory_e
#define LZZ_f00291_rhfactory_e
#define LZZ_INLINE inline
class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  WebSocketServer * ws_ptr;
  RequestHandlerFactory (WebSocketServer * _ws_ptr);
  HTTPRequestHandler * createRequestHandler (HTTPServerRequest const & request);
};
#undef LZZ_INLINE
#endif
// f00292_wsserver.e
//

#ifndef LZZ_f00292_wsserver_e
#define LZZ_f00292_wsserver_e
#define LZZ_INLINE inline
class WebSocketServer : public Poco::Util::ServerApplication
{
public:
  bool dataReady;
  bool isWorking;
  bool isJSON;
  int MAX_FRAME_SIZE;
  char * recBuffer;
  char * okBuffer;
  int recBufferLength;
  JSONValue * recMessage;
  WebSocketServer ();
  ~ WebSocketServer ();
protected:
  void initialize (Application & self);
  void uninitialize ();
  void defineOptions (OptionSet & options);
  void handleOption (std::string const & name, std::string const & value);
  void displayHelp ();
  int main (std::vector <std::string> const & args);
private:
  bool _helpRequested;
};
#undef LZZ_INLINE
#endif
// f00300_singleton.e
//

#ifndef LZZ_f00300_singleton_e
#define LZZ_f00300_singleton_e
#define LZZ_INLINE inline
class Singleton
{
public:
  E_OBJ activeObject;
  E_OBJ tempObj;
  E_MOUSE_STATE mouseState;
  E_GRASS_STATE grassState;
  eProgramState programState;
  eProgramAction (progActionsDown) [E_PS_SIZE*256];
  eProgramAction (progActionsUp) [E_PS_SIZE*256];
  bool isFullScreen;
  bool changesMade;
  bool bufferInvalid;
  bool forceGetPD;
  bool mouseLeftDown;
  bool mouseRightDown;
  bool pboSupported;
  bool notQuit;
  bool timerNotSet;
  bool lbDown;
  bool rbDown;
  bool mbDown;
  bool isZooming;
  bool isPanning;
  bool (keyDownArr) [MAX_KEYS];
  bool wsBufferInvalid;
  bool softMode;
  bool isBare;
  bool reportPagesDrawn;
  int poolItemsCreated;
  int baseW;
  int baseH;
  int scaleFactor;
  int activeMode;
  int visPageSizeInPixels;
  int visPageSizeInUnits;
  int unitSizeInPixels;
  int bufferMult;
  int maxHeightInUnits;
  int extraRad;
  int defaultWinW;
  int defaultWinH;
  int shadersAreLoaded;
  int readyToRecompile;
  int lastPosX;
  int lastPosY;
  int frameCount;
  int maxH;
  int maxW;
  int screenWidth;
  int screenHeight;
  int mouseMovingSize;
  int mouseMovingLoc;
  int mouseMovingStepsBack;
  int mouseCount;
  int lastMouseX;
  int lastMouseY;
  float curBrushRad;
  float diskOn;
  float grassHeight;
  float currentFBOResolutionX;
  float currentFBOResolutionY;
  float mouseX;
  float mouseY;
  float mouseXUp;
  float mouseYUp;
  float cameraZoom;
  float targetZoom;
  float curTime;
  float lastTime;
  float myDelta;
  float mdTime;
  float muTime;
  FIVector4 activeObjectPos;
  FIVector4 minBoundsInPixels;
  FIVector4 maxBoundsInPixels;
  FIVector4 mouseUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseMovePD;
  FIVector4 worldSizeInPages;
  FIVector4 cameraPos;
  FIVector4 lightPos;
  FIVector4 mouseStart;
  FIVector4 mouseEnd;
  FIVector4 * mouseMoving;
  FIVector4 mouseVel;
  FIVector4 worldSeed;
  FIVector4 fogPos;
  FIVector4 bufferDim;
  FIVector4 bufferDimHalf;
  FIVector4 origin;
  FIVector4 lastModXYZ;
  FIVector4 panMod;
  string curShader;
  string allText;
  list <int> pagePoolIds;
  vector <PooledResource*> pagePoolItems;
  vector <string> shaderStrings;
  vector <string> fboStrings;
  vector <string> shaderTextureIDs;
  map <string, Shader*> shaderMap;
  map <string, FBOSet*> fboMap;
  GLuint volTris;
  GLuint grassTris;
  uint * lookup2to3;
  GLuint lookup2to3ID;
  WebSocketServer * myWS;
  Timer myTimer;
  GameWorld * gw;
  Singleton ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS);
  void sendPoolIdToFront (int id);
  int requestPoolId (int requestingPageId);
  static void qNormalizeAngle (int & angle);
  void setupLookups ();
  void perspectiveProjection ();
  void orthographicProjection ();
  ~ Singleton ();
  float genRand (float LO, float HI);
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void keySetup ();
  void createGrassList ();
  void drawCubeCentered (FIVector4 originVec, float radius);
  void drawBox (FIVector4 minVec, FIVector4 maxVec);
  void createVTList ();
  void doShaderRefresh ();
  void setMatrices (int w, int h);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos, int offset = 0);
  void unsampleFBODirect (FBOSet * fbos, int offset = 0);
  void bindFBODirect (FBOSet * fbos);
  void sampleFBO (string fboName, int offset = 0);
  void unsampleFBO (string fboName, int offset = 0);
  FBOWrapper * getFBOWrapper (string fboName, int offset);
  void bindFBO (string fboName);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderFloat (string paramName, float x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderTexture (uint texID, int multitexNumber);
  void setShaderTexture3D (uint texID, int multitexNumber);
  bool shiftDown ();
  bool ctrlDown ();
  bool altDown ();
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  void moveCamera (FIVector4 * modXYZ);
  void moveObject (float dx, float dy, float zoom);
  void doAction (eProgramAction pa);
  void processSpecialKeys (int key, int _x, int _y);
  void processKey (unsigned char key, int _x, int _y, bool isPressed);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  int clamp (int val, int min, int max);
  float clampf (float val, float min, float max);
  void getPixData (FIVector4 * toVector, int xv, int yv);
  void mouseMove (int _x, int _y);
  void worldToScreen (FIVector4 * sc, FIVector4 * wc);
  void screenToWorld (FIVector4 * tc, FIVector4 * wc);
  void mouseClick (int button, int state, int _x, int _y);
  void processB64 ();
  void processJSON ();
  void display ();
  void reshape (int w, int h);
  void idleFunc ();
};
#undef LZZ_INLINE
#endif
// f00310_pooledresource.e
//

#ifndef LZZ_f00310_pooledresource_e
#define LZZ_f00310_pooledresource_e
#define LZZ_INLINE inline
class PooledResource
{
public:
  FBOSet * fboSet;
  uint volID;
  uint volIDLinear;
  int bufferedPageSizeInUnits;
  int usedByPageId;
  Singleton * singleton;
  PooledResource ();
  void init (Singleton * _singleton);
};
#undef LZZ_INLINE
#endif
// f00350_gamepage.e
//

#ifndef LZZ_f00350_gamepage_e
#define LZZ_f00350_gamepage_e
#define LZZ_INLINE inline
class GamePage : public Poco::Runnable
{
public:
  Singleton * singleton;
  int thisPageId;
  int usingPoolId;
  PooledResource * gpuRes;
  int threshVal;
  int bufferedPageSizeInUnits;
  FIVector4 offsetInUnits;
  int iVolumeSize;
  uint * volData;
  uint * volDataLinear;
  int maxHeightInUnits;
  int totLenO2;
  int totLenVisO2;
  FIVector4 worldSeed;
  bool isDirty;
  bool threadRunning;
  float unitSizeInPixels;
  E_STATES curState;
  E_STATES nextState;
  FIVector4 worldMin;
  FIVector4 worldMax;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  E_FILL_STATE fillState;
  GamePage ();
  void init (Singleton * _singleton, int _thisPageId, FIVector4 * _offsetInUnits);
  uint NumberOfSetBits (uint i);
  uint clamp (uint val);
  void createSimplexNoise ();
  void unbindGPUResources ();
  void copyToTexture ();
  void generateVolume ();
  ~ GamePage ();
  void run ();
};
#undef LZZ_INLINE
#endif
// f00380_gameworld.e
//

#ifndef LZZ_f00380_gameworld_e
#define LZZ_f00380_gameworld_e
#define LZZ_INLINE inline
class GameWorld
{
public:
  int pageCount;
  int visPageSizeInUnits;
  int iVolumeSize;
  int ((diagrams) [E_RENDER_LENGTH]) [E_STATE_LENGTH];
  int * curDiagram;
  int renderMethod;
  bool doDrawFBO;
  bool lastProcResult;
  bool updatePoolOrder;
  vector <int> ocThreads;
  FIVector4 lScreenCoords;
  FIVector4 aoScreenCoords;
  FIVector4 worldSizeInPages;
  FIVector4 curPos;
  FIVector4 camPagePos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 lastPagePos;
  FIVector4 tempVec;
  FIVector4 unitPosMin;
  FIVector4 unitPosMax;
  FIVector4 unitPosMinIS;
  FIVector4 unitPosMaxIS;
  FIVector4 startBounds;
  FIVector4 endBounds;
  Singleton * singleton;
  GamePage * * worldData;
  int iBufferSize;
  Poco::ThreadPool threadpool;
  int maxThreads;
  int availThreads;
  int visPageSizeInPixels;
  GameWorld ();
  bool checkBounds (int i, int j, int k);
  void resetToState (E_STATES resState);
  void init (Singleton * _singleton);
  void update ();
  bool processPages ();
  void renderPages ();
  void drawPage (GamePage * gp, int dx, int dy, int dz);
  void combineBuffers ();
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction);
  void renderWorldSpace ();
  void renderGrass ();
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 
// f00290_wsrequesthandler.h
//

#include "f00290_wsrequesthandler.e"
#define LZZ_INLINE inline
WebSocketRequestHandler::WebSocketRequestHandler (WebSocketServer * _ws_ptr)
                                                          {

		ws_ptr = _ws_ptr;
	}
WebSocketRequestHandler::~ WebSocketRequestHandler ()
                                   {
		
		
		
	}
void WebSocketRequestHandler::handleRequest (HTTPServerRequest & request, HTTPServerResponse & response)
        {

		int flags;
		int n;

		//JSONValue *value;

		if (ws_ptr->isWorking) {
			// drop the request, client should only send one request
			// at a time until it receives a response message
			return;
		}


		ws_ptr->isWorking = true;
		ws_ptr->dataReady = false;
		if (ws_ptr->recMessage != NULL) {
			delete ws_ptr->recMessage;
			ws_ptr->recMessage = NULL;
		}

		Application& app = Application::instance();

		
		
		try
		{


			WebSocket ws(request, response);
			app.logger().information("WebSocket connection established.");
			//char buffer[1024];
			
			
			
			do
			{
				n = ws.receiveFrame(ws_ptr->recBuffer, ws_ptr->MAX_FRAME_SIZE, flags);
				app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
				

				if (n > 0) {

					ws_ptr->recBuffer[n+1] = '\0';
					ws_ptr->recBufferLength = n; 

					if (ws_ptr->recBuffer[0] == '{') {
						ws_ptr->isJSON = true;

						ws_ptr->recMessage = JSON::Parse(ws_ptr->recBuffer);

						if (ws_ptr->recMessage == NULL) {
							doTrace("Invalid JSON\n\n");
							ws_ptr->isWorking = false;
							
						}
						else {
							doTrace("");
							doTrace("Valid JSON");
							doTrace("");
							//ws_ptr->isWorking = false;
							ws_ptr->dataReady = true;
						}

					}
					else {
						ws_ptr->isJSON = false;
						ws_ptr->dataReady = true;

					}

					
					

				}
				//01234567890123
				//{"cc":"SENDING"
				
				/*ws_ptr->recBuffer[7] = 'R';
				ws_ptr->recBuffer[8] = 'E';
				ws_ptr->recBuffer[9] = 'C';
				ws_ptr->recBuffer[10] = '_';
				ws_ptr->recBuffer[11] = '_';
				ws_ptr->recBuffer[12] = 'O';
				ws_ptr->recBuffer[13] = 'K';
				*/
				//ws.sendFrame(ws_ptr->recBuffer, n, flags);
				//1234567890123456
				//{"cc":"REC__OK"}
				ws.sendFrame(ws_ptr->okBuffer,16,flags);

			}
			while ( (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE) && PROG_ACTIVE );
			app.logger().information("WebSocket connection closed.");

			

			


		}
		catch (WebSocketException& exc)
		{

			app.logger().log(exc);
			switch (exc.code())
			{
			case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
				// fallthrough
			case WebSocket::WS_ERR_NO_HANDSHAKE:
			case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}


		

		


		

	}
#undef LZZ_INLINE
 
// f00291_rhfactory.h
//

#include "f00291_rhfactory.e"
#define LZZ_INLINE inline
RequestHandlerFactory::RequestHandlerFactory (WebSocketServer * _ws_ptr)
                                                        {
		ws_ptr = _ws_ptr;
	}
HTTPRequestHandler * RequestHandlerFactory::createRequestHandler (HTTPServerRequest const & request)
        {
		Application& app = Application::instance();
		app.logger().information("Request from " 
			+ request.clientAddress().toString()
			+ ": "
			+ request.getMethod()
			+ " "
			+ request.getURI()
			+ " "
			+ request.getVersion());
			
		for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
		{
			app.logger().information(it->first + ": " + it->second);
		}

		doTrace("REQUEST HANDLER CREATED");

		return new WebSocketRequestHandler(ws_ptr);
	}
#undef LZZ_INLINE
 
// f00292_wsserver.h
//

#include "f00292_wsserver.e"
#define LZZ_INLINE inline
WebSocketServer::WebSocketServer ()
  : _helpRequested (false)
        {
		recMessage = NULL;
		dataReady = false;
		isWorking = false;
		isJSON = false;
		recBufferLength = 0;
		
		MAX_FRAME_SIZE = 16777216; //16 MB
		recBuffer = new char[MAX_FRAME_SIZE];
		okBuffer = "{\"cc\":\"REC__OK\"}";
		
	}
WebSocketServer::~ WebSocketServer ()
        {
		if (recBuffer != NULL) {
			delete[] recBuffer;
		}
	}
void WebSocketServer::initialize (Application & self)
        {
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
void WebSocketServer::uninitialize ()
        {
		ServerApplication::uninitialize();
	}
void WebSocketServer::defineOptions (OptionSet & options)
        {
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}
void WebSocketServer::handleOption (std::string const & name, std::string const & value)
        {
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}
void WebSocketServer::displayHelp ()
        {
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
		helpFormatter.format(std::cout);
	}
int WebSocketServer::main (std::vector <std::string> const & args)
        {





		// get parameters from configuration file
		unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
		
		// set-up a server socket
		ServerSocket svs(port);
		// set-up a HTTPServer instance
		HTTPServer srv(new RequestHandlerFactory(this), svs, new HTTPServerParams);
		// start the HTTPServer
		srv.start();
		// wait for CTRL-C or kill
		//waitForTerminationRequest();
		// Stop the HTTPServer

		glutMainLoop();
		
		srv.stop();


		return Application::EXIT_OK;

	}
#undef LZZ_INLINE
 
// f00300_singleton.h
//

#include "f00300_singleton.e"
#define LZZ_INLINE inline
Singleton::Singleton ()
                    {
		volTris = NULL;
		gw = NULL;
		myWS = NULL;
	}
void Singleton::init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS)
                                                                                                {

		pushTrace("Singleton init");



		mouseMovingSize = 100;
		mouseMovingLoc = 0;
		mouseCount = 0;
		mouseMovingStepsBack = 20;
		mouseMoving = new FIVector4[mouseMovingSize];

		myWS = _myWS;

		poolItemsCreated = 0;
		activeMode = 1;

		isZooming = false;
		isPanning = false;
		softMode = false;
		reportPagesDrawn = false;
		isBare = false;
		grassHeight = 1.0/128.0;


		defaultWinW = _defaultWinW/_scaleFactor;
		defaultWinH = _defaultWinH/_scaleFactor;
		scaleFactor = _scaleFactor;

		curBrushRad = 1.0f;

		mouseState = E_MOUSE_STATE_MOVE;

		worldSeed.setFXYZ(

			genRand(5000.0f,500000.0f),
			genRand(5000.0f,500000.0f),
			genRand(5000.0f,500000.0f)
		);

		
		bufferMult = 2;
		diskOn = 0.0f;
		visPageSizeInPixels = 128; // height of one page in pixels
		visPageSizeInUnits = 8;
		worldSizeInPages.setIXYZ(128,128,12);
		unitSizeInPixels = (visPageSizeInPixels)/visPageSizeInUnits;
		

		maxH = worldSizeInPages.getIZ();
		maxW = 4;

		maxHeightInUnits = (worldSizeInPages.getIZ()-bufferMult)*(visPageSizeInUnits);

		minBoundsInPixels.setIXYZ(0,0,0);
		maxBoundsInPixels.setIXYZ(
			(worldSizeInPages.getIX()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIY()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIZ()-1)*unitSizeInPixels*visPageSizeInUnits
		);


		wsBufferInvalid = true;

		bufferDim.setIXY(defaultWinW,defaultWinH);
		bufferDimHalf.setIXY(defaultWinW/2,defaultWinH/2);
		

		myTimer.start();

		grassState = E_GRASS_STATE_OFF;

		activeObject = E_OBJ_NONE;

		extraRad = 0;
		lastTime = 0.0;

		srand(time(0));

		


		int i;
		mbDown=false;
		lbDown=false;
		rbDown=false;

		for (i = 0; i < MAX_KEYS; i++) {
			keyDownArr[i] = false;
		}
		keySetup();

		

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;


		cameraPos.setFXYZ(2048.0, 2048.0, maxBoundsInPixels.getFZ()/2.0);

		lightPos.copyFrom(&cameraPos);
		fogPos.copyFrom(&cameraPos);

		lightPos.setFZ(maxBoundsInPixels.getFZ()*0.6f);
		//lightPos.addXYZ(0.0f, 0.0f, 0.0f);
		fogPos.addXYZ(-256.0f);

	    cameraZoom = 1.0f;
	    targetZoom = 1.0f;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;

		createVTList();
		createGrassList();


		//// GL WIDGET START ////
		
		frameCount = 0;
		changesMade = false;
		forceGetPD = false;
		bufferInvalid = false;
		notQuit = true;
		timerNotSet = true;
		screenWidth = 400;
		screenHeight = 300;
		mouseLeftDown = mouseRightDown = false;
		mouseX = mouseY = 0;
		myDelta = 0.0f;
		
		
		

		

	


		//gm = new GameMap();
		orthographicProjection();
		//// GL WIDGET END ////



	    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment
	    glEnable(GL_TEXTURE_2D);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);
	    glDisable(GL_LIGHTING);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glDisable(GL_BLEND);
	    //glEnable(GL_BLEND);


	    


	    fboStrings.push_back("worldSpaceFBO");

	    fboStrings.push_back("pagesFBO");
	    fboStrings.push_back("grassFBO");
	    fboStrings.push_back("geomFBO");
	    fboStrings.push_back("combineFBO");
	    fboStrings.push_back("resultFBO");
	    fboStrings.push_back("volGenFBO");


	    /*
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("CalcFlow");
	    shaderStrings.push_back("Erode");
	    shaderStrings.push_back("DLA");
	    shaderStrings.push_back("MapShader");
	    shaderStrings.push_back("shaderWater");
	    */


	    shaderStrings.push_back("WorldSpaceShader");
	    shaderStrings.push_back("BlitShader");
	    shaderStrings.push_back("LightingShader");
	    shaderStrings.push_back("GeomShader");
	    shaderStrings.push_back("GrassShader");
	    shaderStrings.push_back("CombineShader");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("GenerateVolumeBare");
	    shaderStrings.push_back("RenderVolume");
	    

	    

	    shaderTextureIDs.push_back("Texture0");
	    shaderTextureIDs.push_back("Texture1");
	    shaderTextureIDs.push_back("Texture2");
	    shaderTextureIDs.push_back("Texture3");
	    shaderTextureIDs.push_back("Texture4");
	    shaderTextureIDs.push_back("Texture5");
	    shaderTextureIDs.push_back("Texture6");
	    shaderTextureIDs.push_back("Texture7");


	    setupLookups();

	    for (i = 0; i < shaderStrings.size(); i++) {
	        shaderMap.insert(  pair<string,Shader*>(shaderStrings[i], NULL)  );
	    }
	    doShaderRefresh();

	    //fboSize = 512;
	    //bufsPerFBO = 2;

	    for (i = 0; i < fboStrings.size(); i++) {
	        fboMap.insert(  pair<string, FBOSet*>(fboStrings[i], new FBOSet())  );
	    }

	    //init(int _numBufs, int _width, int _height, int _bytesPerChannel);

	    fboMap["worldSpaceFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false);

	    fboMap["pagesFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["grassFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["geomFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, true);
	    fboMap["combineFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["resultFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["volGenFBO"]->init(1, 4096, 4096, 1, false);


	    gw = new GameWorld();
	    gw->init(this);


	    
	    popTrace();




	}
void Singleton::sendPoolIdToFront (int id)
                                       {
		int i = 0;

		pagePoolIds.remove(id);
		pagePoolIds.push_front(id);

	}
int Singleton::requestPoolId (int requestingPageId)
                                                {

		
		int pageToFreeId;
		int usedByPageId;

		
		if (TOT_GPU_MEM_USAGE < MAX_GPU_MEM) {
			pagePoolItems.push_back( new PooledResource() );
			pagePoolItems.back()->init(this);

			pageToFreeId = poolItemsCreated;
			pagePoolIds.push_front(pageToFreeId);
			poolItemsCreated++;

		}
		else {
			pageToFreeId = pagePoolIds.back();
			usedByPageId = pagePoolItems[pageToFreeId]->usedByPageId;

			GamePage* consumingPage;

			if (usedByPageId == -1) {
				// this pooledItem is already free 

			}
			else {
				// free this pooledItem from the page that is consuming it and give it to the requesting page

				consumingPage = gw->worldData[usedByPageId];

				if (consumingPage == NULL) {
					// page was deleted already
				}
				else {
					consumingPage->unbindGPUResources();
				}
				

			}

			pagePoolIds.pop_back();
			pagePoolIds.push_front(pageToFreeId);
		}


		pagePoolItems[pageToFreeId]->usedByPageId = requestingPageId;

		return pageToFreeId;

	}
void Singleton::qNormalizeAngle (int & angle)
        {
	    while (angle < 0) {
	        angle += 360 * 16;
	    }
	    while (angle > 360 * 16) {
	        angle -= 360 * 16;
	    }
	}
void Singleton::setupLookups ()
                            {
		pushTrace("setupLookups");

	    uint i, j, k, m;
	    uint side = 256;
	    uint totalSize = side*side*side;

	    lookup2to3 = new uint[totalSize];
	    //lookup3to2 = new uint[totalSize];

	    uint xpos;
	    uint ypos;
	    uint zpos;

	    uint ind;

	    ind = 0;
	    for (j = 0; j < 4096; j++) {
	        for (i = 0; i < 4096; i++) {
	            ind = i+j*4096;
	            xpos = i%256;
	            ypos = j%256;
	            zpos = i/256 + (j/256)*16;////(ind)/(256*256);

	            //lookup2to3[ind] = ind;
	            //ind++;
	            lookup2to3[ind] = (255<<24)|(zpos<<16)|(ypos<<8)|xpos;   
	        }
	    }
	    
	    /*
	    ind = 0;
	    for (k = 0; k < side; k++) {
	        for (j = 0; j < side; j++) {
	            for (i = 0; i < side; i++) {

	                lookup3to2[ind] = ind;
	                ind++;
	            }
	        }
	    }
	    */
	    

	    /*
	    glGenTextures(1,&lookup3to2ID);
	    glBindTexture(GL_TEXTURE_3D,lookup3to2ID);
	    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, side, side, side, 0, GL_RGBA, GL_UNSIGNED_BYTE, lookup3to2);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
	    
	    
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	    
	    glBindTexture(GL_TEXTURE_3D,0);
	    */
	    /*
	    glBindTexture(GL_TEXTURE_2D,texID);
	        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iRenderSize, iRenderSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, uTexMap);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        glBindTexture(GL_TEXTURE_2D,0);
	    */

	    glGenTextures(1,&lookup2to3ID);
	    glBindTexture(GL_TEXTURE_2D,lookup2to3ID);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, lookup2to3);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
	    /*
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    */
	    glBindTexture(GL_TEXTURE_2D,0);

	    //delete[] lookup2to3;
	    //delete[] lookup3to2;

	    //lookup2to3 = NULL;
	    //lookup3to2 = NULL;


	    popTrace();

	}
void Singleton::perspectiveProjection ()
        {
	    float aspect = 1.0;

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glFrustum(-aspect, +aspect, -1.0, +1.0, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	}
void Singleton::orthographicProjection ()
        {

	    glMatrixMode (GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();

	}
Singleton::~ Singleton ()
                     {
		if (gw) {
			delete gw;
		}
	}
float Singleton::genRand (float LO, float HI)
                                          {

		return LO + (float)rand()/((float)RAND_MAX/(HI-LO));
	}
void Singleton::setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown)
                                                                                               {

		if (isDown) {
			progActionsDown[ps*256 + kc] = pa;
		}
		else {
			progActionsUp[ps*256 + kc] = pa;
		}
	}
void Singleton::setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown)
                                                                                {
		int i;

		for (i = 0; i < E_PS_SIZE; i++) {
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

	}
void Singleton::keySetup ()
                        {

		setProgActionAll(27,  E_PA_QUIT, false);
		setProgActionAll('p', E_PA_TOGGLE_FULLSCREEN, false);
		setProgActionAll('r', E_PA_REFRESH, false);

	}
void Singleton::createGrassList ()
                               {

		int i;
		int j;

		float fi;
		float fj;

		float tcx;
		float tcy;

		int spacing = 1;

		int iMax = 512;
		int jMax = 512;

		float fiMax = (float)iMax;
		float fjMax = (float)jMax;
		float heightMod;

		grassTris = glGenLists(1);
		

		glNewList(grassTris, GL_COMPILE);

		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);

		//glNormal3f(0, 0, 1);

		

		for (j = jMax-1; j >= 0; j -= spacing) {
			fj = ((float)(j*2-jMax) + 1.0f)/fjMax;
			tcy = fj;//(fj + 1.0f)/2.0f;
			for (i = 0; i < iMax; i += spacing) {
				fi = ((float)(i*2-iMax) + 1.0f)/fiMax;
				tcx = fi;//(fi + 1.0f)/2.0f;
			

				heightMod = 0.0;//genRand(0.0f,4.0f)/fjMax;

				//glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

				//
				
				
				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.2f, -1.0);
				glVertex3f(fi,fj,0.0);
				

				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.0f, 0.0);
				glVertex3f(fi,fj,0.0f);


				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.2f, 1.0);
				glVertex3f(fi,fj,0.0f);


				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 1.0f, 0.0);
				glVertex3f(fi,fj,0.0f);

				
			}


			
		}

		glEnd();
		
		glEndList();
		
	}
void Singleton::drawCubeCentered (FIVector4 originVec, float radius)
                                                                 {
		FIVector4 minV;
		FIVector4 maxV;

		minV.setFXYZ(
			originVec.getFX()-radius,
			originVec.getFY()-radius,
			originVec.getFZ()-radius
		);

		maxV.setFXYZ(
			originVec.getFX()+radius,
			originVec.getFY()+radius,
			originVec.getFZ()+radius
		);

		drawBox(minV,maxV);
	}
void Singleton::drawBox (FIVector4 minVec, FIVector4 maxVec)
                                                     {


		float minX = minVec.getFX();
		float minY = minVec.getFY();
		float minZ = minVec.getFZ();

		float maxX = maxVec.getFX();
		float maxY = maxVec.getFY();
		float maxZ = maxVec.getFZ();


    	glBegin(GL_QUADS);

    	    // front
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    // back
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    // right
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    // left
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    // top
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    // bottom
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);



    	glEnd();
    }
void Singleton::createVTList ()
                            {

		volTris = glGenLists(1);
		

		float texMin = 0.0f;
		float texMax = 1.0f;

		float fx1 = -1.0f;
		float fy2 = -1.0f;
		float fx2 = 1.0f;
		float fy1 = 1.0f;

		float centerX = (fx1 + fx2)/2.0f;
		float centerY = (fy1 + fy2)/2.0f;

		float fy25 = fy1*0.75f + fy2*0.25f;
		float fy75 = fy1*0.25f + fy2*0.75f;

		float coordsX[8];
		float coordsY[8];

		coordsX[0] = centerX;
		coordsY[0] = centerY;

		coordsX[1] = centerX;
		coordsY[1] = fy1;

		coordsX[2] = fx2;
		coordsY[2] = fy25;

		coordsX[3] = fx2;
		coordsY[3] = fy75;

		coordsX[4] = centerX;
		coordsY[4] = fy2;

		coordsX[5] = fx1;
		coordsY[5] = fy75;

		coordsX[6] = fx1;
		coordsY[6] = fy25;

		coordsX[7] = coordsX[1];
		coordsY[7] = coordsY[1];

		float backfaceX[8];
		float backfaceY[8];
		float backfaceZ[8];



		backfaceX[0] = texMin;
		backfaceY[0] = texMin;
		backfaceZ[0] = texMin;
		backfaceX[1] = texMin;
		backfaceY[1] = texMin;
		backfaceZ[1] = texMax;
		backfaceX[2] = texMax;
		backfaceY[2] = texMin;
		backfaceZ[2] = texMax;
		backfaceX[3] = texMax;
		backfaceY[3] = texMin;
		backfaceZ[3] = texMin;
		backfaceX[4] = texMax;
		backfaceY[4] = texMax;
		backfaceZ[4] = texMin;
		backfaceX[5] = texMin;
		backfaceY[5] = texMax;
		backfaceZ[5] = texMin;
		backfaceX[6] = texMin;
		backfaceY[6] = texMax;
		backfaceZ[6] = texMax;
		backfaceX[7] = backfaceX[1];
		backfaceY[7] = backfaceY[1];
		backfaceZ[7] = backfaceZ[1];


		glNewList(volTris, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 0, 1);

		int i;

		for (i = 0; i < 8; i++) {

			glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

			glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);
			
			if (i == 0) {
				//glColor4f((backfaceX[i]+1.0f)/2.0f, (backfaceY[i]+1.0f)/2.0f, (backfaceZ[i]+1.0f)/2.0f, 1.0f);
				glMultiTexCoord3f( GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
			}
			else {
				glMultiTexCoord3f( GL_TEXTURE1, backfaceX[i], backfaceY[i], backfaceZ[i]);
			}

			glVertex3f(coordsX[i],coordsY[i],0.0f);
		}

		glEnd();
		

		glEndList();
	}
void Singleton::doShaderRefresh ()
                               {
		pushTrace( "doShaderRefresh" );

	    readyToRecompile = 0;

	    int i;
	    int j;


	    for (i = 0; i < shaderStrings.size(); i++) {
	        if (shaderMap[ shaderStrings[i] ]) {
	            delete shaderMap[ shaderStrings[i] ];
	        }
	        shaderMap[ shaderStrings[i] ] = new Shader( ("../src/glsl/"+shaderStrings[i]+".c").c_str() );
	    }
	    shadersAreLoaded=1;
	    readyToRecompile = 1;

	    popTrace();
	    
	}
void Singleton::setMatrices (int w, int h)
                                       {

		glViewport(0,0,w,h);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();

	}
void Singleton::setWH (int w, int h)
                                 {

	    baseW = w;
	    baseH = h;
	}
void Singleton::sampleFBODirect (FBOSet * fbos, int offset)
                                                         {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = 0; i < fbos->numBufs; i++) {
	            setShaderTexture(fbos->fbos[i].color_tex, i+offset);
	        }
	    }
	}
void Singleton::unsampleFBODirect (FBOSet * fbos, int offset)
                                                           {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(0, i+offset);
	        }
	    }
	}
void Singleton::bindFBODirect (FBOSet * fbos)
                                         {
	    setMatrices(fbos->width,fbos->height);
	    fbos->bind(1);
	    currentFBOResolutionX = fbos->width;
	    currentFBOResolutionY = fbos->height;
	}
void Singleton::sampleFBO (string fboName, int offset)
                                                     {
	    FBOSet* fbos = fboMap[fboName];
	    sampleFBODirect(fbos,offset);
	}
void Singleton::unsampleFBO (string fboName, int offset)
                                                       {
	    
	    FBOSet* fbos = fboMap[fboName];
	    unsampleFBODirect(fbos,offset);
	}
FBOWrapper * Singleton::getFBOWrapper (string fboName, int offset)
                                                               {
		FBOSet* fbos = fboMap[fboName];
		return fbos->getFBOWrapper(offset);
	}
void Singleton::bindFBO (string fboName)
                                     {
	    
	    FBOSet* fbos = fboMap[fboName];
	    bindFBODirect(fbos);
	}
void Singleton::unbindFBO ()
                         {
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	    setMatrices(baseW,baseH);
	    
	}
void Singleton::bindShader (string shaderName)
                                           {

	    if (shadersAreLoaded) {
	        curShader = shaderName;
	        shaderMap[curShader]->bind();
	    }
	    
	}
void Singleton::unbindShader ()
                            {
	    if (shadersAreLoaded) {
	        shaderMap[curShader]->unbind();
	    }
	    
	}
void Singleton::setShaderFloat (string paramName, float x)
                                                       {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x);

	        shaderMap[curShader]->setShaderFloat(paramName, x);
	    }
	}
void Singleton::setShaderfVec2 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec2(paramName, v);
	    }
	}
void Singleton::setShaderVec2 (string paramName, float x, float y)
                                                               {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec2(paramName, x, y);
	    }
	}
void Singleton::setShaderVec3 (string paramName, float x, float y, float z)
                                                                        {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec3(paramName, x, y, z);
	    }
	}
void Singleton::setShaderfVec3 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec3(paramName, v);
	    }
	}
void Singleton::setShaderVec4 (string paramName, float x, float y, float z, float w)
                                                                                 {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y,z,w);
	        shaderMap[curShader]->setShaderVec4(paramName, x, y, z, w);
	    }
	}
void Singleton::setShaderfVec4 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec4(paramName, v);
	    }
	}
void Singleton::setShaderTexture (uint texID, int multitexNumber)
                                                              {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_2D, texID);
	        shaderMap[curShader]->setShaderInt(shaderTextureIDs[multitexNumber] ,multitexNumber);
	    }
	}
void Singleton::setShaderTexture3D (uint texID, int multitexNumber)
                                                                {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_3D, texID);
	        shaderMap[curShader]->setShaderInt(shaderTextureIDs[multitexNumber],multitexNumber);
	    }
	}
bool Singleton::shiftDown ()
                         {
		return glutGetModifiers()&GLUT_ACTIVE_SHIFT;
	}
bool Singleton::ctrlDown ()
                        {
		return glutGetModifiers()&GLUT_ACTIVE_CTRL;
	}
bool Singleton::altDown ()
                       {
		return glutGetModifiers()&GLUT_ACTIVE_ALT;
	}
void Singleton::drawFSQuad (float zoom)
                                    {
	    drawFSQuadOffset(0.0f,0.0f,zoom);
	}
void Singleton::drawFSQuadOffset (float xOff, float yOff, float zoom)
                                                                  {
	    float fx1 = (xOff-1.0f)*zoom;
	    float fy1 = (yOff-1.0f)*zoom;
	    float fx2 = (xOff+1.0f)*zoom;
	    float fy2 = (yOff+1.0f)*zoom;

	    glColor4f(1, 1, 1, 1);
	    glBegin(GL_QUADS);
	    glNormal3f(0, 0, 1);
	    
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(fx1,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(fx2,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(fx2,fy2,0.0f);
	    
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(fx1,fy2,0.0f);
	    
	    glEnd();
	}
void Singleton::drawFBO (string fboName, int ind, float zoom)
                                                          {
	    drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
	}
void Singleton::drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom)
                                                                                            {

	    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
	    //glClearColor(0.2,0.2,0.2,0.0);
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    drawFSQuadOffset(xOff,yOff,zoom);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}
void Singleton::drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom)
                                                                                        {
	    FBOSet* fbos = fboMap[fboName];
	    drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
	}
void Singleton::moveCamera (FIVector4 * modXYZ)
                                           {
		wsBufferInvalid = true;
		cameraPos.addXYZRef(modXYZ);
		modXYZ->setFZ(0.0f);
		lightPos.addXYZRef(modXYZ, 1.0f);
		fogPos.addXYZRef(modXYZ, 1.0f);
		isPanning = true;
	}
void Singleton::moveObject (float dx, float dy, float zoom)
                                                        {


		

		float dxZoom = dx/zoom;
		float dyZoom = dy/zoom;

		bool doDefault = false;

		FIVector4 modXYZ;

		if (lbDown||rbDown) {
			if (rbDown || (shiftDown() ) ) {
				modXYZ.setFZ(  dyZoom );
				modXYZ.setFX( -(0.0f + dxZoom/2.0f) );
				modXYZ.setFY( -(0.0f - dxZoom/2.0f) );



			}
			else {
				modXYZ.setFX( -(dyZoom + dxZoom/2.0f) );
				modXYZ.setFY( -(dyZoom - dxZoom/2.0f) );
			}

			//modXYZTemp.copyFrom(&modXYZ);
			//modXYZTemp.normalize();
			lastModXYZ.addXYZRef(&modXYZ);


			
			
		}
		else {
			
		}

		

		if (shiftDown()) {

			if (mouseState == E_MOUSE_STATE_BRUSH) {
				curBrushRad -= modXYZ.getFZ()/50.0f;

				if (curBrushRad < 0.0f) {
					curBrushRad = 0.0f;
				}
			}
			else {
				grassHeight -= modXYZ.getFZ()/10000.0f;

				if (grassHeight < 0.0f) {
					grassHeight = 0.0f;
				}
			}

			
		}
		else {


			if (mouseState == E_MOUSE_STATE_BRUSH) {
				doDefault = true;
			}
			else {
				switch (activeObject) {

					case E_OBJ_LIGHT:
						lightPos.addXYZRef(&modXYZ, -1.0f);
						//lightPos.clampXYZ(&minBoundsInPixels,&maxBoundsInPixels);
						activeObjectPos.setFXYZRef(&lightPos);
					break;

					case E_OBJ_FOG:
						fogPos.addXYZRef(&modXYZ, -1.0f);
						//lightPos.clampXYZ(&minBoundsInPixels,&maxBoundsInPixels);
						activeObjectPos.setFXYZRef(&fogPos);
					break;
					default:
						doDefault = true;
					break;

				}
			}

			if (doDefault) {
				moveCamera(&modXYZ);
			}


			
		}


		

		

	}
void Singleton::doAction (eProgramAction pa)
                                         {
		switch (pa) {
			case E_PA_QUIT:
				if (isFullScreen) {
					//glutLeaveGameMode();
				}
				PROG_ACTIVE = false;
				glutLeaveMainLoop();
			break;
			case E_PA_TOGGLE_FULLSCREEN:
				isFullScreen = !isFullScreen;

				if (isFullScreen) {
					//glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
					//glutEnterGameMode();
					//glutFullScreen();
				}
				else {
					//glutReshapeWindow(defaultWinW, defaultWinH);
					//glutLeaveGameMode();
				}

			break;
			case E_PA_REFRESH:
            	doShaderRefresh();
			break;
			case E_PA_SIZE:
				//unused
			break;

		}
	}
void Singleton::processSpecialKeys (int key, int _x, int _y)
                                                         {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		switch(key) {
			case GLUT_KEY_F1 :

			break;
			case GLUT_KEY_F2 :

			break;
			case GLUT_KEY_F3 :

			break;
		}
	}
void Singleton::processKey (unsigned char key, int _x, int _y, bool isPressed)
                                                                           {
		
		int x = _x/scaleFactor;
		int y = _y/scaleFactor;
	}
void Singleton::keyboardUp (unsigned char key, int _x, int _y)
                                                           {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		changesMade = false;


		//doAction(progActionsUp[((int)programState)*256 + key]);

		int enCounter;

		bool restartGen = false;

		//doTrace(i__s(key) );


		if (key == 17) {
			glutLeaveMainLoop();
		}

		switch(key ) {

			
			case '0':
				activeMode = 0;

			break;
			case '1':
				activeMode = 1;
				doTrace("Dirt and Grass");
			break;
			case '2':
				activeMode = 2;
				doTrace("Rock");
			break;
			case '3':
				activeMode = 3;
				doTrace("Brick");
			break;
			case '4':
				activeMode = 4;
				doTrace("Flat Top Stone");
			break;
			case '5':
				activeMode = 5;
				doTrace("Flat Top Dirt");
			break;
			case '6':
				activeMode = 6;
			break;
			case '7':
				activeMode = 7;
			break;
			case '8':
				activeMode = 8;
			break;
			case '9':
				activeMode = 9;
			break;

			case 's':
				softMode = !softMode;
				
			break;

			case 'w':
				changesMade = true;
				maxW++;
			break;
			case 'q':
				changesMade = true;
				maxW--;
				if (maxW < 1) {
					maxW = 1;
				}
			break;

			case 'r':
				doShaderRefresh();
				bufferInvalid = true;

			break;

			case 'g':

				enCounter = (int)grassState;
				enCounter++;
				grassState = (E_GRASS_STATE)enCounter;

				if (grassState == E_GRASS_STATE_LENGTH) {
					grassState = (E_GRASS_STATE)0;
				}

				bufferInvalid = true;
				changesMade = true;
			break;



			case 't':
				restartGen = true;
			break;

			case '\t':


				enCounter = (int)mouseState;
				
				

				if (ctrlDown()) {
					enCounter--;
				} 
				else {
					enCounter++;
				}
				if (enCounter < 0) {
					enCounter = ((int)E_MOUSE_STATE_LENGTH)-1;
				}

				mouseState = (E_MOUSE_STATE)enCounter;

				if (mouseState == E_MOUSE_STATE_LENGTH) {
					mouseState = (E_MOUSE_STATE)0;
				}

				bufferInvalid = true;
				changesMade = true;
				wsBufferInvalid = true;
				forceGetPD = true;


			break;

			case 'c':
				isBare = !isBare;
				restartGen = true;
			break;

			case 'm':
				reportPagesDrawn = true;
				//doTrace("Avail threads: ", i__s(gw->availThreads));
			break;

			case 'a':
				changesMade = true;
				maxH++;
			break;
			case 'z':
				changesMade = true;
				maxH--;
				if (maxH < 0) {
					maxH = 0;
				}
			break;
			
			default:
				
			break;
		}

		if (restartGen) {
			doShaderRefresh();
			gw->resetToState(E_STATE_CREATESIMPLEXNOISE_END);
			bufferInvalid = true;
			changesMade = true;
		}

	}
void Singleton::keyboardDown (unsigned char key, int _x, int _y)
                                                             {


		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		//doAction(progActionsDown[((int)programState)*256 + key]);
	}
int Singleton::clamp (int val, int min, int max)
                                             {
		if (val > max) {
			val = max;
		}
		if (val < min) {
			val = min;
		}
		return val;
	}
float Singleton::clampf (float val, float min, float max)
                                                      {
		if (val > max) {
			val = max;
		}
		if (val < min) {
			val = min;
		}
		return val;
	}
void Singleton::getPixData (FIVector4 * toVector, int xv, int yv)
                                                             {

		FBOWrapper* fbow;
		int newX;
		int newY;

		float newFX;
		float newFY;

		if (wsBufferInvalid) {
			gw->renderWorldSpace();
		}

		


		if (cameraZoom > 1.0) {
			newX = (int)((  (((float)xv)-bufferDimHalf.getFX()) / cameraZoom)+bufferDimHalf.getFX());
			newY = (int)((  (((float)yv)-bufferDimHalf.getFY()) / cameraZoom)+bufferDimHalf.getFY());
		}
		else {
			newX = clamp(xv,0,bufferDim.getIX()-1);
			newY = clamp(yv,0,bufferDim.getIY()-1);
		}

		

		fbow = getFBOWrapper("worldSpaceFBO",0);
		fbow->getPixelAtF(toVector, newX, (bufferDim.getIY()-1)-newY);
	}
void Singleton::mouseMove (int _x, int _y)
                                       {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		int dx = x - lastPosX;
		int dy = y - lastPosY;


		lastMouseX = x;
		lastMouseY = y;


		

		mouseXUp = x;
		mouseYUp = y;

		

		if (lbDown || rbDown) {
		    moveObject((float)dx, (float)dy, cameraZoom);
		}
		else {
			if (mouseState == E_MOUSE_STATE_BRUSH  ) {
				getPixData(&mouseMovePD, x, y);
				gw->modifyUnit(&mouseMovePD, E_BRUSH_MOVE);
			}
		}


		if (mbDown) {

		}
		lastPosX = x;
		lastPosY = y;

		if ( (x >= 0) && (y >= 0) && (x < baseW) && (y < baseH) ) { // && (rbDown||lbDown||mbDown)
			bufferInvalid = true;
			
			if (rbDown||lbDown) {
				changesMade = true;
			}
			
			
		}
	}
void Singleton::worldToScreen (FIVector4 * sc, FIVector4 * wc)
                                                         {

		FIVector4 dMod;

		dMod.copyFrom(wc);
		dMod.addXYZRef(&cameraPos, -1.0);

		float x1 = (dMod.getFX()-dMod.getFY());
		float y1 = (-(dMod.getFX()/2.0f) + -(dMod.getFY()/2.0f) + dMod.getFZ());
		float myZoom = std::min(1.0f, cameraZoom);

		x1 = x1*(myZoom)/bufferDimHalf.getFX();
		y1 = y1*(myZoom)/bufferDimHalf.getFY();

		x1 = (x1 + 1.0)/2.0;
		y1 = (y1 + 1.0)/2.0;

		sc->setFX(x1);
		sc->setFY(y1);
	}
void Singleton::screenToWorld (FIVector4 * tc, FIVector4 * wc)
                                                         {


	}
void Singleton::mouseClick (int button, int state, int _x, int _y)
                                                               {
		

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		lastPosX = x;
		lastPosY = y;

		int res;

		float wheelDelta = 0.0;
		bool mbClicked = false;
		bool rbClicked = false;
		bool lbClicked = false;

		int mouseMovingStart;
		


		switch (button) {
			case GLUT_LEFT_BUTTON:
				lbDown = (state == GLUT_DOWN);
				lbClicked = (state == GLUT_UP);
				
			break;
			case GLUT_RIGHT_BUTTON:
				rbDown = (state == GLUT_DOWN);
				rbClicked = (state == GLUT_UP);

			break;
			case GLUT_MIDDLE_BUTTON:
				mbDown = (state == GLUT_DOWN);
				mbClicked = (state == GLUT_UP);

			break;

			case 3: // wheel up
				wheelDelta = 1.0/20.0f;
				//changesMade = true;
			break;

			case 4: // wheel down
				wheelDelta = -1.0/20.0f;
				//changesMade = true;
			break;
		}








		if (state == GLUT_DOWN) {
			mouseVel.setFXY(0.0f,0.0f);
		}

		if (mbClicked) {

		}
		

		if (rbDown || lbDown) {
			
		}
		else {

		}

		if (rbClicked || lbClicked) {

			


			if (rbDown || lbDown) {

			}
			else {

				doTrace("MOUSE UP");
				muTime = myTimer.getElapsedTimeInMilliSec();

				mouseEnd.setIXY(x,y);
				//mouseVel.copyFrom(&mouseEnd);
				//mouseVel.addXYZRef(&mouseMoving, -1.0f);

				if (activeObject != E_OBJ_LIGHT && activeObject != E_OBJ_FOG && (muTime-mdTime > 300.0f) ) {
					mouseMovingStart = (mouseMovingLoc - min(mouseMovingStepsBack, mouseCount) + mouseMovingSize)%mouseMovingSize;
					

					if (mouseCount > 0) {
						mouseVel.copyFrom( &(mouseMoving[mouseMovingLoc]) );
						mouseVel.addXYZRef( &(mouseMoving[mouseMovingStart]), -1.0f);
					}

					
				}

				

				
				/*
				mouseMovingSize = 100;
				mouseMovingLoc = 0;
				mouseCount = 0;
				mouseMovingStepsBack = 10;
				*/

				lastModXYZ.normalize();

				//if (activeObject == E_OBJ_LIGHT || activeObject == E_OBJ_CAMERA) {
					//mouseVel
				//}


				//mouseVel.multXYZ( clampf(1.0f-(muTime-mdTime)/1000.0f, 0.1f, 1.0f)/cameraZoom );

				if (shiftDown()) {

				}
				else {
					




					activeObject = E_OBJ_NONE;
					wsBufferInvalid = true;
					getPixData(&mouseUpPD, x, y);

					
					



					if ( mouseEnd.distance(&mouseStart) > 30.0 ) {
						
					} 
					else {
						if (mouseState == E_MOUSE_STATE_BRUSH) {


							if (lbClicked) {
								gw->modifyUnit(&mouseUpPD, E_BRUSH_ADD);
							}
							else {
								gw->modifyUnit(&mouseUpPD, E_BRUSH_SUB);
							}

							forceGetPD = true;

						}
					}


					diskOn = 0.0f;
				}

				
			}

			
		}
		else {
			if (rbDown || lbDown) {

				if (rbDown && lbDown) {

				}
				else {


					lastModXYZ.setFXYZ(0.0f, 0.0f, 0.0f);

					mouseMovingLoc = 0;
					mouseCount = 0;
					doTrace("MOUSE DOWN");
					mdTime = myTimer.getElapsedTimeInMilliSec();
					mouseStart.setIXY(x,y);

					getPixData(&mouseDownPD, x, y);
					activeObject = (E_OBJ)((int) mouseDownPD.getFW());

					switch (activeObject) {
						case E_OBJ_LIGHT:
							activeObjectPos.setFXYZRef(&lightPos);
							diskOn = 1.0f;
						break;

						case E_OBJ_FOG:
							activeObjectPos.setFXYZRef(&fogPos);
							diskOn = 1.0f;
						break;
						default:
							
						break;
					}

					if (mouseState == E_MOUSE_STATE_BRUSH) {
						diskOn = 0.0;
					}


				}

				
			}
		}


		









		
		

		if (button == 3 || button == 4) {

			myDelta += wheelDelta;
			targetZoom = pow(2.0, myDelta);
			isZooming = true;

		}

		if (x >= 0 && y >= 0 && x < baseW && y < baseH) {
			bufferInvalid = true;
		}

	}
void Singleton::processB64 ()
                          {
		doTrace("processData() BASE64");

		membuf sbuf(myWS->recBuffer, myWS->recBuffer + myWS->recBufferLength);
		Poco::Base64Decoder b64in(sbuf);

		//ofBuffer buffer;
		//b64in >> buffer;



	}
void Singleton::processJSON ()
                           {
		doTrace("processData() JSON");

		JSONValue *jsonVal = myWS->recMessage;

		if (jsonVal == NULL) {

		}
		else {
			//doTrace( "JSON VAL", f__s(jsonVal->Child(L"x")->number_value) , "\n\n" );
		}
		
		
	}
void Singleton::display ()
                           {





		curTime = myTimer.getElapsedTimeInMilliSec();

		float elTime = curTime - lastTime;
		float dz;
		float fMouseVel;

		if (myWS == NULL) {

		}
		else {

			if (myWS->dataReady) {

				if (myWS->isJSON) {
					processJSON();
				}
				else {
					processB64();
				}

				myWS->dataReady = false;
				myWS->isWorking = false;
			}
		}
		

		


		if (elTime >= 16.0f) {
			lastTime = curTime;



			mouseMovingLoc = (mouseMovingLoc+1)%mouseMovingSize;
			mouseCount++;
			mouseMoving[mouseMovingLoc].setIXY(lastMouseX,lastMouseY);



			lastModXYZ.multXYZ(0.95f);

			mouseVel.multXYZ(0.95f);
			
			fMouseVel = mouseVel.distance(&origin);

			if ( fMouseVel < 1.0f ) {
				mouseVel.setFXY(0.0f,0.0f);
				isPanning = false;
			}
			else {
				isPanning = true;


				panMod.copyFrom(&lastModXYZ);
				panMod.multXYZ(fMouseVel/16.0f);
				moveCamera(&panMod);
			}


			dz = (targetZoom-cameraZoom)/(16.0f);

			if (abs(dz) < 0.0001) {
				dz = 0.0f;
			}

			if (cameraZoom > 8.0f) {
				cameraZoom = 8.0f;
			}
			if (cameraZoom < 1.0f/8.0f) {
				cameraZoom = 1.0f/8.0f;
			}
			
			cameraZoom += dz;

			if ((dz == 0.0f) && (isZooming)) {
				isZooming = false;
				wsBufferInvalid = true;
				bufferInvalid = true;
				changesMade = true;
			}
			else {
				if (isZooming) {
					bufferInvalid = true;
					changesMade = true;
				}
				
			}

			if (shadersAreLoaded) {
				gw->update();

				changesMade = false;
				bufferInvalid = false;
			}
		}

		

		

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}
void Singleton::reshape (int w, int h)
                                   {
		
		setWH(w,h);

		screenWidth = w;
		screenHeight = h;
		
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		orthographicProjection();

		
		setMatrices(baseW,baseH);
	}
void Singleton::idleFunc ()
                            {
		
	}
#undef LZZ_INLINE
 
// f00310_pooledresource.h
//

#include "f00310_pooledresource.e"
#define LZZ_INLINE inline
PooledResource::PooledResource ()
                         {
		fboSet = NULL;
	}
void PooledResource::init (Singleton * _singleton)
                                         {

		singleton = _singleton;
		usedByPageId = -1;

		

		bufferedPageSizeInUnits = (singleton->visPageSizeInUnits) * (singleton->bufferMult);

		fboSet = new FBOSet();
		fboSet->init(2,(singleton->visPageSizeInPixels)*2,(singleton->visPageSizeInPixels)*2,1,false);
		glGenTextures(1,&volID);
		glGenTextures(1,&volIDLinear);

		glBindTexture(GL_TEXTURE_3D,volID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, bufferedPageSizeInUnits, bufferedPageSizeInUnits, bufferedPageSizeInUnits, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);

		glBindTexture(GL_TEXTURE_3D,volIDLinear);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, bufferedPageSizeInUnits, bufferedPageSizeInUnits, bufferedPageSizeInUnits, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);

		TOT_GPU_MEM_USAGE += ((float)(bufferedPageSizeInUnits*bufferedPageSizeInUnits*bufferedPageSizeInUnits*4*2))/(1024.0f*1024.0f);

	}
#undef LZZ_INLINE
 
// f00350_gamepage.h
//

#include "f00350_gamepage.e"
#define LZZ_INLINE inline
GamePage::GamePage ()
                   {

	}
void GamePage::init (Singleton * _singleton, int _thisPageId, FIVector4 * _offsetInUnits)
                                                                                     {
		int i;

		thisPageId = _thisPageId;

		singleton = _singleton;

		usingPoolId = -1;

		threshVal = 140;


		threadRunning = false;


		maxHeightInUnits = (singleton->maxHeightInUnits);

		isDirty = false;

		fillState = E_FILL_STATE_PARTIAL;

		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;





		bufferedPageSizeInUnits = (singleton->visPageSizeInUnits) * (singleton->bufferMult);
		offsetInUnits.copyFrom(_offsetInUnits);

		unitSizeInPixels = (float)(singleton->unitSizeInPixels);


		worldSeed.copyFrom(&(singleton->worldSeed));



		iVolumeSize = bufferedPageSizeInUnits*bufferedPageSizeInUnits*bufferedPageSizeInUnits;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}

		volDataLinear = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volDataLinear[i] = (255<<24)|(255<<16)|(255<<8)|(0);
		}


		totLenO2 = bufferedPageSizeInUnits/2;
		totLenVisO2 = bufferedPageSizeInUnits/(2*(singleton->bufferMult));



		worldMin.copyFrom(&offsetInUnits);
		worldMax.copyFrom(&offsetInUnits);
		worldMin.addXYZ( -totLenVisO2 );
		worldMax.addXYZ(  totLenVisO2 );
		worldMin.multXYZ((float)unitSizeInPixels);
		worldMax.multXYZ((float)unitSizeInPixels);


		worldUnitMin.copyFrom(&offsetInUnits);
		worldUnitMax.copyFrom(&offsetInUnits);
		worldUnitMin.addXYZ( -totLenO2 );
		worldUnitMax.addXYZ(  totLenO2 );

		

		curState = E_STATE_INIT_END;
	}
uint GamePage::NumberOfSetBits (uint i)
        {
	    i = i - ((i >> 1) & 0x55555555);
	    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
uint GamePage::clamp (uint val)
                             {
		if (val > 255) {
			val = 255;
		}
		if (val < 0) {
			val = 0;
		}
		return val;
	}
void GamePage::createSimplexNoise ()
                                  {

		threadRunning = true;

		// REMINDER: DO NOT ACCESS EXTERNAL POINTERS INSIDE THREAD

		bool isBlank = false;
		bool isFull = false;
		

		curState = E_STATE_CREATESIMPLEXNOISE_BEG;

		int i, j, k, m;

		int totLen = bufferedPageSizeInUnits;

		float fTotLen = (float)totLen;

		int ind = 0;

		uint tmp;

		float fx, fy, fz;

		uint randOff[3];
		float ijkVals[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};

		float totLenO4 = totLen/4;
		float totLen3O4 = (totLen*3)/4;
		float fSimp;
		float heightThresh;
		float testVal;

		bool mustNotBeFull = false;

		for (j = 0; j < totLen; j++) {

			ijkVals[1] = (float)j;

			fy = (j - totLenO2) + offsetInUnits.getFY();

			for (i = 0; i < totLen; i++) {

				ijkVals[0] = (float)i;

				fx = (i - totLenO2) + offsetInUnits.getFX();
				
				for (k = 0; k < totLen; k++) {

					ijkVals[2] = (float)k;

					fz = (k - totLenO2) + offsetInUnits.getFZ();

					ind = k*totLen*totLen + j*totLen + i;

					
					
					if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
						volData[ind] = 0;
						volDataLinear[ind] = (255<<16)|(255<<8)|255;
					}
					else {

						heightThresh = (fz/ ((float)maxHeightInUnits) );
						if (heightThresh > 1.0f) {
							heightThresh = 1.0f;
						}
						if (heightThresh < 0.0f) {
							heightThresh = 0.0f;
						}



						if (k + offsetInUnits.getIZ() >= maxHeightInUnits) {
							tmp = 0;
							mustNotBeFull = true;
						}
						else {
							testVal = simplexScaledNoise(
												4.0f, //octaves
												0.5f, //persistence (amount added in each successive generation)
												1.0f/32.0f, //scale (frequency)
												0.0f,
												1.0f,
												fx+worldSeed.getFX(),
												fy+worldSeed.getFY(),
												fz+worldSeed.getFZ()
											);
							

							
							tmp = clamp(testVal*255.0f*(1.0f-heightThresh*heightThresh*heightThresh));
						}

						

						if ( i >= totLenO4 && i <= totLen3O4 ) {
							if ( j >= totLenO4 && j <= totLen3O4 ) {
								if ( k >= totLenO4 && k <= totLen3O4 ) {
									if (tmp > threshVal) {
										isBlank = false;
									}
									else {
										isFull = false;
									}
								}
							}
						}

						
						


						for (m = 0; m < 3; m++) {
							fSimp = simplexScaledNoise(
																		1.0f, //octaves
																		1.0f, //persistence (amount added in each successive generation)
																		1.0f/4.0, //scale (frequency)
																		0.0f,
																		1.0f,
																		fx+RAND_MOD[m*3+0],
																		fy+RAND_MOD[m*3+1],
																		fz+RAND_MOD[m*3+2]
																	);
							randOff[m] = clamp( ( fSimp + ijkVals[m])*255.0f/fTotLen);
							


						}

						if ( (tmp%16 > 5) && ( (i+j+k)%2 == 0) ) {

							/*if (randOff[0] == 0 && randOff[1] == 0 && randOff[2] == 0) {
								randOff[1] = 1;
							} */

							volData[ind] = (0)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
							volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;
						}
						else {
							volData[ind] = (0);
							volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;;
						}

						
					}
					
					

					
				}
			}
		}



		
		if (mustNotBeFull) {
			isFull = false;
		}

		if (isBlank || isFull ) {

			if (isBlank) {
				fillState = E_FILL_STATE_EMPTY;
			}
			if (isFull) {
				fillState = E_FILL_STATE_FULL;
			}

			curState = E_STATE_LENGTH;
		}
		else {

			fillState = E_FILL_STATE_PARTIAL;
			curState = E_STATE_CREATESIMPLEXNOISE_END;
		}

		threadRunning = false;

	}
void GamePage::unbindGPUResources ()
                                  {
		usingPoolId = -1;
		curState = E_STATE_CREATESIMPLEXNOISE_END;
	}
void GamePage::copyToTexture ()
                             {

		curState = E_STATE_COPYTOTEXTURE_BEG;


		if (usingPoolId == -1) {
			usingPoolId = singleton->requestPoolId(thisPageId);
			gpuRes = singleton->pagePoolItems[usingPoolId];
		}

		

		glBindTexture(GL_TEXTURE_3D,gpuRes->volID);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volData
			);

		glBindTexture(GL_TEXTURE_3D,0);
		glBindTexture(GL_TEXTURE_3D,gpuRes->volIDLinear);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volDataLinear
			);
		glBindTexture(GL_TEXTURE_3D,0);

		

		curState = E_STATE_COPYTOTEXTURE_END;

	}
void GamePage::generateVolume ()
                              {

		curState = E_STATE_GENERATEVOLUME_BEG;
		
		
		if (usingPoolId == -1) {

		}
		else {
			if (singleton->isBare) {
				singleton->bindShader("GenerateVolumeBare");
			}
			else {
				singleton->bindShader("GenerateVolume");
			}



			singleton->bindFBO("volGenFBO");
			singleton->setShaderTexture3D(gpuRes->volID, 0);
			singleton->setShaderTexture3D(gpuRes->volIDLinear, 1);
			singleton->setShaderTexture(singleton->lookup2to3ID, 2);

			singleton->setShaderFloat("bufferedPageSizeInUnits", bufferedPageSizeInUnits);
			singleton->setShaderFloat("threshVal", (float)threshVal);
			singleton->setShaderfVec3("worldMin", &(worldMin));
			singleton->setShaderfVec3("worldMax", &(worldMax));

			singleton->drawFSQuad(1.0f);

			singleton->setShaderTexture3D(0, 0);
			singleton->setShaderTexture3D(0, 1);
			singleton->setShaderTexture(0, 2);

			singleton->unbindFBO();
			singleton->unbindShader();

			//ray trace new texture, generate normals, AO, depth, etc
			singleton->bindShader("RenderVolume");
			singleton->bindFBODirect(gpuRes->fboSet);
			singleton->sampleFBO("volGenFBO");
			glClearColor(0.0f,0.0f,0.0f,0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			singleton->setShaderfVec3("worldMin", &(worldMin));
			singleton->setShaderfVec3("worldMax", &(worldMax));


			singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
			

			glCallList(singleton->volTris);
			singleton->unsampleFBO("volGenFBO");
			singleton->unbindFBO();
			singleton->unbindShader();
		}

		curState = E_STATE_GENERATEVOLUME_END;
	}
GamePage::~ GamePage ()
                    {

		if (volData) {
			delete[] volData;
		}
		if (volDataLinear) {
			delete[] volDataLinear;
		}
	}
void GamePage::run ()
                   {
		switch (nextState) {
			case E_STATE_CREATESIMPLEXNOISE_LAUNCH:
				createSimplexNoise();
			break;

			default:

			break;
		}
	}
#undef LZZ_INLINE
 
// f00380_gameworld.h
//

#include "f00380_gameworld.e"
#define LZZ_INLINE inline
GameWorld::GameWorld ()
        {

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

	}
bool GameWorld::checkBounds (int i, int j, int k)
                                              {
		
		bool res = true;

		if (i < 0) {res = false;}
		if (j < 0) {res = false;}
		if (k < 0) {res = false;}
		if (i >= worldSizeInPages.getIX()) {res = false;}
		if (j >= worldSizeInPages.getIY()) {res = false;}
		if (k >= worldSizeInPages.getIZ()) {res = false;}

		return res;
	}
void GameWorld::resetToState (E_STATES resState)
                                             {
		int i;

		threadpool.joinAll();

		for (i = 0; i < iVolumeSize; i++) {
			if (worldData[i]) {
				if( worldData[i]->curState > resState) {

					if (worldData[i]->fillState == E_FILL_STATE_PARTIAL) {
						worldData[i]->curState = resState;
					}

					
				}
			}
		}
	}
void GameWorld::init (Singleton * _singleton)
                                         {

		#ifdef DEBUG_MODE
		pushTrace("GameWorld init");
		#endif

		int i;
		int j;

		updatePoolOrder = false;

		pageCount = 0;
		lastProcResult = true;
		maxThreads = 7;
		availThreads = maxThreads;

		for (i = 0; i < maxThreads; i++) {
			ocThreads.push_back(-1);
		}

		

		

		doDrawFBO = false;

		for (j = 0; j < E_RENDER_LENGTH; j++) {
			for (i = 0; i < E_STATE_LENGTH; i++) {
				diagrams[j][i] = E_STATE_WAIT;
			}
		}

		renderMethod = (int)E_RENDER_VOL;
		singleton = _singleton;
		worldSizeInPages.copyFrom( &(singleton->worldSizeInPages) );

		visPageSizeInPixels = singleton->visPageSizeInPixels;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_COPYTOTEXTURE_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_GENERATEVOLUME_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_GENERATEVOLUME_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		visPageSizeInUnits = singleton->visPageSizeInUnits;

		iVolumeSize = worldSizeInPages.getIX()*worldSizeInPages.getIY()*worldSizeInPages.getIZ();
		
	    worldData = new GamePage*[iVolumeSize];
	    
	      

		
		for (i = 0; i < iVolumeSize; i++) {
			worldData[i] = NULL;
		}

	    
	    #ifdef DEBUG_MODE
	    popTrace();
	    #endif
	}
void GameWorld::update ()
                      {

		bool changesMade = singleton->changesMade;
		bool bufferInvalid = singleton->bufferInvalid;

		bool procResult;
		bool doRenderGeom = false;


		if (singleton->isZooming || singleton->isPanning ) {
			
		}
		else {
			procResult = processPages();
			
			if ( (lastProcResult != procResult) && (procResult == false)  ) {
				singleton->wsBufferInvalid = true;

				updatePoolOrder = true;

			}
		}

			if (procResult || changesMade) {
				renderPages();

				
				
				if ( (singleton->grassState != E_GRASS_STATE_ANIM) ) {
					renderGrass();
					doRenderGeom = true;

				}

				
			}

			if (singleton->grassState == E_GRASS_STATE_ANIM) {
				renderGrass();
				doRenderGeom = true;
				bufferInvalid = true;
			}
		

			


		if (procResult || changesMade || bufferInvalid || singleton->rbDown || singleton->lbDown) {


			if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
				doRenderGeom = true;
			}

			if (doRenderGeom) {
				renderGeom();
				combineBuffers();
			}
			

			glClearColor(0.6,0.6,0.7,0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			postProcess();
			
			glutSwapBuffers();
			glFlush();
			
		}

		if (singleton->forceGetPD) {
			singleton->forceGetPD = false;
			renderWorldSpace();
		}
	


	


		lastProcResult = procResult;
	}
bool GameWorld::processPages ()
                            {


		
		int counter;
		int i, j, k;
		int res;
		int ind;

		int iw = worldSizeInPages.getIX();
	    int jw = worldSizeInPages.getIY();
	    int kw = worldSizeInPages.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    bool cmade = false;




	    camPagePos.copyFrom( &(singleton->cameraPos) );
		camPagePos.intDivXYZ(visPageSizeInPixels);

	    int curInd = 0;
		

	    int m;
	    E_STATES nState;

	    int loadRad = singleton->maxW;

	    int changeCount = 0;
	    int maxChanges = 32;

	    if (singleton->lbDown || singleton->rbDown) {
	    	loadRad = 1;
	    	//return false;
	    }
	    


	    // check for threads to free
	    if (availThreads < maxThreads) {
	    	for (i = 0; i < ocThreads.size(); i++) {
	    		if ( ocThreads[i] == -1) {
	    			// already freed
	    		}
	    		else{
	    			if ( worldData[ocThreads[i]] == NULL ) {
	    				// page was destroyed, free thread

	    				ocThreads[i] = -1;
	    				availThreads++;
	    			}
	    			else {
	    				if (worldData[ocThreads[i]]->threadRunning) {

	    				}
	    				else {
	    					ocThreads[i] = -1;
	    					availThreads++;
	    				}
	    			}
	    		}
	    	}
	    }

	    if (availThreads == 0) {
	    	return false;
	    }


		for (k = 0; k < singleton->maxH; k++) {
	    //for (k = -loadRad; k <= loadRad; k++) {
	    	kk = k;//+camPagePos.getIZ();

			for (j = -(loadRad); j <= (loadRad); j++) {
				jj = j+camPagePos.getIY();

				for (i = -(loadRad); i <= (loadRad); i++) {
					ii = i+camPagePos.getIX();

					
					

					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						curPos.setIXYZ(ii*visPageSizeInUnits,jj*visPageSizeInUnits,kk*visPageSizeInUnits);

						


						if (worldData[ind] == NULL) {

							#ifdef DEBUG_MODE
							doTrace("E_STATE_INIT_LAUNCH");
							#endif

							worldData[ind] = new GamePage();
							worldData[ind]->init(singleton, ind, &curPos);

							pageCount++;
							changeCount++;


						}
						else {

							
							/*

							if (worldData[ind]->curState == E_STATE_CREATESIMPLEXNOISE_END_NOT_PARTIAL) {
								availThreads++;
								if (availThreads > maxThreads) {
									availThreads = maxThreads;
								}
								worldData[ind]->curState = E_STATE_LENGTH;
							}

							if (worldData[ind]->curState == E_STATE_CREATESIMPLEXNOISE_END_PARTIAL) {
								availThreads++;
								if (availThreads > maxThreads) {
									availThreads = maxThreads;
								}
								worldData[ind]->curState = E_STATE_CREATESIMPLEXNOISE_END;
								
							}

							*/

							nState = (E_STATES)curDiagram[worldData[ind]->curState];


							switch(nState) {
	                            case E_STATE_CREATESIMPLEXNOISE_LAUNCH:

	                            	#ifdef DEBUG_MODE
	                            	doTrace("E_STATE_CREATESIMPLEXNOISE_LAUNCH");
	                            	#endif

	                            	

	                            	if ( availThreads > 0 ) {

	                            		

	                            		counter = 0;
	                            		while (ocThreads[counter] != -1) {
	                            			counter++;
	                            			if (counter >= maxThreads) {
	                            				break;
	                            			}
	                            		}
	                            		if (counter >= maxThreads) {
	                            			
	                            		}
	                            		else {


	                            			ocThreads[counter] = ind;
	                            			
	                            			availThreads--;

	                            			worldData[ind]->nextState = nState;

	                            			try {
	                            				threadpool.start(*worldData[ind]);
	                            			}
	                            			catch (SystemException & exc) {
	                            				doTrace("MEM EXCEPTION");
	                            			}

	                            			changeCount++;

	                            		}


	                            		
	                            	}
	                            	else {
	                            		return cmade;
	                            	}
	                            	

	                            	//worldData[ind]->run();
									
									
								break;
								case E_STATE_COPYTOTEXTURE_LAUNCH:

									

									#ifdef DEBUG_MODE
									doTrace("E_STATE_COPYTOTEXTURE_LAUNCH");
									#endif

									
									worldData[ind]->nextState = nState;
									worldData[ind]->copyToTexture();
									cmade = true;
									
									changeCount++;

									//return cmade;
								break;


								case E_STATE_GENERATEVOLUME_LAUNCH:

									#ifdef DEBUG_MODE
									doTrace("E_STATE_GENERATE_VOLUME_LAUNCH");
									#endif

									
									worldData[ind]->nextState = nState;
									worldData[ind]->generateVolume();
									cmade = true;

									changeCount++;
									
									//return cmade;
								break;

								
								case E_STATE_WAIT:

									//do nothing
								break;
								case E_STATE_LENGTH:
	                            	
								break;
								default:
	                                
								break;

							}

							if (changeCount >= maxChanges) {
								return cmade;
							}
							
						}

					}

					curInd++;
				}
			}
		}



		
		return cmade;
	}
void GameWorld::renderPages ()
                           {

		int i, j, k;
		int res;
		int ind;

		int iw = worldSizeInPages.getIX();
	    int jw = worldSizeInPages.getIY();
	    int kw = worldSizeInPages.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    int loadRad = 8;

	    camPagePos.copyFrom( &(singleton->cameraPos) );
	    camPagePos.intDivXYZ(visPageSizeInPixels);

	    int curInd = 0;
		

	    int m;

	    int drawnPageCount = 0;
	    int skippedPages = 0;


	    singleton->bindShader("BlitShader");
	    singleton->bindFBO("pagesFBO");



		for (k = 0; k < singleton->maxH; k++) {
			kk = k;//+camPagePos.getIZ();
		//for (k = -loadRad; k <= loadRad; k++) {
			//kk = k+camPagePos.getIZ();


			for (j = -loadRad; j <= loadRad; j++) {
				jj = j+camPagePos.getIY();

				for (i = -loadRad; i <= loadRad; i++) {
					ii = i+camPagePos.getIX();

					
					

					

					

					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						if (worldData[ind] == NULL) {

						}
						else {

							if (
								(worldData[ind]->fillState == E_FILL_STATE_PARTIAL)
								
							) {
								switch(curDiagram[worldData[ind]->curState]) {
									case E_STATE_LENGTH:

										if (worldData[ind]->usingPoolId == -1) {
											skippedPages++;
										}
										else {
											if (updatePoolOrder) {
												singleton->sendPoolIdToFront(worldData[ind]->usingPoolId);
											}
											drawPage(worldData[ind], ii, jj, kk);
											drawnPageCount++;

											if (drawnPageCount >= singleton->pagePoolIds.size()-2) {

												//doTrace("BROKE EARLY!!! ", i__s(drawnPageCount), " / ", i__s(singleton->pagePoolIds.size()-2));
												//singleton->maxW--;
												goto drawPageExit;
											}

										}

										
									break;
								}
							}

							
							
						}

					}

					curInd++;
				}
			}

			
		}

		drawPageExit:

		singleton->unbindShader();
		singleton->unbindFBO();

		updatePoolOrder = false;

		if (singleton->reportPagesDrawn) {
			singleton->reportPagesDrawn = false;

			doTrace("Pages Drawn: ",i__s(drawnPageCount), " Pages Skipped: ",i__s(skippedPages), " AvailPages: ",i__s(singleton->pagePoolIds.size()-2));
		}
	    

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}
void GameWorld::drawPage (GamePage * gp, int dx, int dy, int dz)
                                                            {

		float pitchSrc = (float)((singleton->visPageSizeInPixels*2));
		float pitchSrc2 = (float)((singleton->visPageSizeInPixels*2)/2);

		float dxmod = dx*pitchSrc2 - singleton->cameraPos.getFX();
		float dymod = dy*pitchSrc2 - singleton->cameraPos.getFY();
		float dzmod = dz*pitchSrc2 - singleton->cameraPos.getFZ();


		float fx1 = (dxmod-dymod) - pitchSrc2;
		float fy1 = (-(dxmod/2.0f) + -(dymod/2.0f) + dzmod) - pitchSrc2;
		float fx2 = fx1 + pitchSrc;
		float fy2 = fy1 + pitchSrc;



		
		// TODO: should be baseW/H?

		float sx = singleton->bufferDimHalf.getFX();
		float sy = singleton->bufferDimHalf.getFY();

		float myZoom = std::min(1.0f,singleton->cameraZoom);


		fx1 = fx1*(myZoom)/sx;
		fy1 = fy1*(myZoom)/sy;
		fx2 = fx2*(myZoom)/sx;
		fy2 = fy2*(myZoom)/sy;



	    singleton->sampleFBODirect(gp->gpuRes->fboSet);


	    glColor4f(1, 1, 1, 1);
	    glBegin(GL_QUADS);
	    glNormal3f(0, 0, 1);
	    
	    
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(fx1,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(fx2,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(fx2,fy2,0.0f);
	    
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(fx1,fy2,0.0f);
	    
	    glEnd();

	    singleton->unsampleFBODirect(gp->gpuRes->fboSet);



	}
void GameWorld::combineBuffers ()
                              {

		singleton->bindShader("CombineShader");

		singleton->bindFBO("combineFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("grassFBO",2);
		//singleton->sampleFBO("geomFBO",4);


		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		
		singleton->drawFSQuad(1.0f);

		//singleton->unsampleFBO("geomFBO",4);
		singleton->unsampleFBO("grassFBO",2);
		singleton->unsampleFBO("pagesFBO",0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
	}
void GameWorld::renderGeom ()
                          {


		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GeomShader");
		
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		
		singleton->bindFBO("geomFBO");
		//singleton->sampleFBO("pagesFBO");


		glEnable(GL_DEPTH_TEST);
		//remember 2x radius


		switch (singleton->mouseState) {
			case E_MOUSE_STATE_BRUSH:
				singleton->setShaderFloat("matVal", 6.0f);
				singleton->drawCubeCentered(lastUnitPos, ((int)singleton->curBrushRad)*(singleton->unitSizeInPixels)  );
				glClear(GL_DEPTH_BUFFER_BIT);
			break;
			case E_MOUSE_STATE_OBJECTS:
				singleton->setShaderFloat("matVal", 4.0f);
				singleton->drawCubeCentered(singleton->lightPos,32.0);

				singleton->setShaderFloat("matVal", 5.0f);
				singleton->drawCubeCentered(singleton->fogPos,32.0);
			break;
			
		}
		

		glDisable(GL_DEPTH_TEST);

		

		//singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		
	}
void GameWorld::modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction)
                                                                               {

		int radius = ((int)singleton->curBrushRad);

		FIVector4 fPixelWorldCoords;
		fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);

		/*
		if (brushAction == E_BRUSH_ADD) {
			fPixelWorldCoords.addXYZ(0.0,0.0,radius*singleton->unitSizeInPixels);
		}
		if (brushAction == E_BRUSH_SUB) {
			fPixelWorldCoords.addXYZ(0.0,0.0,-radius*singleton->unitSizeInPixels);
		}
		*/

		

		


		int iw = worldSizeInPages.getIX();
		int jw = worldSizeInPages.getIY();
		int kw = worldSizeInPages.getIZ();
		int ind;
		int ind2;
		int pageSizeMult = visPageSizeInUnits*(singleton->bufferMult);


		int i, j, k, m, n, o, p;

		int ii;
		int jj;
		int kk;

		int pixelPS = (singleton->unitSizeInPixels*singleton->visPageSizeInUnits);



		lastUnitPos.copyFrom(&fPixelWorldCoords);
		lastUnitPos.intDivXYZ(singleton->unitSizeInPixels);
		lastUnitPos.multXYZ(singleton->unitSizeInPixels);
		lastUnitPos.setFW(singleton->unitSizeInPixels);

		lastPagePos.copyFrom(&fPixelWorldCoords);
		lastPagePos.intDivXYZ(pixelPS);
		lastPagePos.multXYZ(pixelPS);
		lastPagePos.setFW(pixelPS);

		bool changes = false;

		int newRad = 2 + radius/visPageSizeInUnits;


		uint linV;
		uint nearV;

		uint linR;
		uint linG;
		uint linB;
		uint linA;

		uint nearR;
		uint nearG;
		uint nearB;
		uint nearA;

		bool isInside;


		pagePos.copyFrom(&fPixelWorldCoords);
		unitPos.copyFrom(&fPixelWorldCoords);

		pagePos.intDivXYZ( pixelPS );
		unitPos.intDivXYZ( singleton->unitSizeInPixels );


		unitPosMin.copyFrom(&unitPos);
		unitPosMax.copyFrom(&unitPos);

		unitPosMin.addXYZ((float)radius-1, -1.0f);
		unitPosMax.addXYZ((float)radius+1, 1.0f);

		unitPosMinIS.copyFrom(&unitPos);
		unitPosMaxIS.copyFrom(&unitPos);
		unitPosMinIS.addXYZ((float)radius+1, -1.0f);
		unitPosMaxIS.addXYZ((float)radius-1, 1.0f);


		if (brushAction == E_BRUSH_MOVE) {
			return;
		}



		
		for (m = 0; m < 2; m++) {
			for (i = -newRad; i <= newRad; i++) {
				for (j = -newRad; j <= newRad; j++) {
					for (k = -newRad; k <= newRad; k++) {




						ii = i+pagePos.getIX();
						jj = j+pagePos.getIY();
						kk = k+pagePos.getIZ();

						if (checkBounds(ii,jj,kk)) {
							ind = kk*jw*iw + jj*iw + ii;

							if (worldData[ind] == NULL) {

								curPos.setIXYZ(ii*visPageSizeInUnits,jj*visPageSizeInUnits,kk*visPageSizeInUnits);
								worldData[ind] = new GamePage();
								worldData[ind]->init(singleton, ind, &curPos);
								worldData[ind]->createSimplexNoise();

								//doTrace("created new page");
								//worldData[ind]->curState = E_STATE_LENGTH;
							}

							if (
								//(worldData[ind]->curState != E_STATE_CREATESIMPLEXNOISE_BEG) // ||
								//(worldData[ind]->curState == E_STATE_LENGTH)
								true
							) {
								

								startBounds.maxXYZ(&unitPosMin,&(worldData[ind]->worldUnitMin));
								endBounds.minXYZ(&unitPosMax,&(worldData[ind]->worldUnitMax));

								

								

								



								if (
									(startBounds.getFX() > endBounds.getFX()) || 
									(startBounds.getFY() > endBounds.getFY()) ||
									(startBounds.getFZ() > endBounds.getFZ())
								) {
									
								}
								else {

									


									for (n = startBounds.getIX(); n < endBounds.getIX(); n++) {
										for (o = startBounds.getIY(); o < endBounds.getIY(); o++) {
											for (p = startBounds.getIZ(); p < endBounds.getIZ(); p++) {

												tempVec.setIXYZ(n,o,p);
												isInside = tempVec.inBoundsXYZ(&unitPosMinIS,&unitPosMaxIS);
												

												if (
													tempVec.inBoundsXYZ(
														&(worldData[ind]->worldUnitMin),
														&(worldData[ind]->worldUnitMax)
													)
												) {
													tempVec.addXYZRef( &(worldData[ind]->worldUnitMin), -1.0);

													ind2 = tempVec.getIZ()*pageSizeMult*pageSizeMult + tempVec.getIY()*pageSizeMult + tempVec.getIX();
													
													if (ind2 < 0 || ind2 >= pageSizeMult*pageSizeMult*pageSizeMult) {
														//doTrace("ind2 out of range ", i__s(ind2), " of ", i__s(pageSizeMult*pageSizeMult*pageSizeMult));
													}
													else {

														if (m == 0) {

															linV = worldData[ind]->volDataLinear[ind2];
															nearV = worldData[ind]->volData[ind2];

															linR = (linV)&255;
															linG = (linV>>8)&255;
															linB = (linV>>16)&255;
															linA = (linV>>24)&255;

															nearR = (nearV)&255;
															nearG = (nearV>>8)&255;
															nearB = (nearV>>16)&255;
															nearA = (nearV>>24)&255;


															if (p >= singleton->maxHeightInUnits) {

																linA = 0;
															}
															else {

																if (brushAction == E_BRUSH_SUB) {


																	if (isInside) {
																		if (singleton->softMode) {
																			linA = min(linA, linA-8);
																		}
																		else {
																			linA = 0;
																		}
																	}
																	
																}
																else {

																	if (isInside) {
																		if (singleton->softMode) {
																			linA += 8;
																			if (linA > 255) {
																				linA = 255;
																			}
																		}
																		else {
																			linA = 255;
																		}
																	}

																	

																	switch(singleton->activeMode) {
																		//
																		case 0:

																		break;

																		// dirt and grass
																		case 1:
																			linR = 255;
																			linG = 255;
																			linB = 255;

																			nearA = 0;
																		break;

																		// rock
																		case 2:
																			linR = 255;
																			linG = 255;
																			linB = 255;

																			nearA = 255;
																		break;
																		
																		// brick
																		case 3:
																			linR = 16;
																			linG = 255;
																			linB = 16;

																			nearA = 255;
																		break;
																			
																		// flat top
																		case 4:
																			linB = 0;
																			nearA = 255;
																		break;
																		
																		//
																		case 5:
																			linB = 0;
																			nearA = 0;
																		break;
																		
																		//
																		case 6:

																		break;
																		
																		//
																		case 7:

																		break;

																		//
																		case 8:

																		break;
																		
																		//
																		case 9:

																		break;
																		
																	}
																}
															}

															worldData[ind]->volData[ind2] = (nearA<<24)|(nearB<<16)|(nearG<<8)|(nearR);
															worldData[ind]->volDataLinear[ind2] = (linA<<24)|(linB<<16)|(linG<<8)|(linR);
															

															worldData[ind]->isDirty = true;
															changes = true;
														}
														else {
															if (worldData[ind]->isDirty) {
																worldData[ind]->isDirty = false;
																worldData[ind]->copyToTexture();
																worldData[ind]->generateVolume();
															}
														}

													}

												}


											}
										}
									}
								}

								


								

							}
							
						}


						




						

						

					}
				}
			}
		}

		if (changes) {
			singleton->changesMade=true;
			singleton->wsBufferInvalid=true;
		}

		
		

	}
void GameWorld::renderWorldSpace ()
                                {
		//doTrace("renderWorldSpace() TOT GPU MEM USED (MB): ", f__s(TOT_GPU_MEM_USAGE));

		singleton->wsBufferInvalid = false;

		singleton->bindShader("WorldSpaceShader");
		

		singleton->bindFBO("worldSpaceFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("geomFBO",2);

		
		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec4("fogPos", &(singleton->fogPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));
		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf) );


		singleton->drawFSQuad(1.0f);

		
		singleton->unsampleFBO("geomFBO",2);
		singleton->unsampleFBO("pagesFBO",0);
		

		singleton->unbindFBO();
		singleton->unbindShader();

		float newZoom = std::max(1.0f,singleton->cameraZoom);
		singleton->drawFBO("resultFBO", 0, newZoom );

		FBOWrapper* fbow = singleton->getFBOWrapper("worldSpaceFBO",0);

		fbow->getPixels();

	}
void GameWorld::renderGrass ()
                           {


		float curTime = 0.0;

		if (singleton->grassState == E_GRASS_STATE_ANIM) {
			curTime = singleton->curTime;
		}

		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GrassShader");
		
		singleton->setShaderFloat("curTime", curTime);
		singleton->setShaderFloat("grassHeight", singleton->grassHeight);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));

		
		singleton->bindFBO("grassFBO");
		singleton->sampleFBO("pagesFBO");

		if (singleton->grassState == E_GRASS_STATE_ANIM || singleton->grassState == E_GRASS_STATE_ON) {
			glCallList(singleton->grassTris);
		}
		

		singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		
	}
void GameWorld::postProcess ()
                           {

		// NOTE: ALWAYS UNSAMPLE IN REVERSE ORDER!!!


		//singleton->drawFBO("worldSpaceFBO", 0, 1.0 );

		float newZoom;

		singleton->worldToScreen(&lScreenCoords, &(singleton->lightPos));
		singleton->worldToScreen(&aoScreenCoords, &(singleton->activeObjectPos));

		singleton->bindShader("LightingShader");
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));
		singleton->setShaderfVec2("lightPosSS", &lScreenCoords);
		singleton->setShaderfVec2("aoPosSS", &aoScreenCoords);
		singleton->setShaderfVec4("fogPos", &(singleton->fogPos));
		singleton->setShaderfVec4("activeObjectPos", &(singleton->activeObjectPos));
		
		singleton->setShaderfVec4("lastUnitPos", &(lastUnitPos) );
		singleton->setShaderfVec4("lastPagePos", &(lastPagePos) );




		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		singleton->setShaderFloat("diskOn",singleton->diskOn);
		singleton->setShaderFloat("curTime", singleton->curTime);

		
		singleton->bindFBO("resultFBO");
		singleton->sampleFBO("combineFBO",0);
		singleton->sampleFBO("geomFBO", 2);

		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

		singleton->drawFSQuad(1.0f);
		singleton->unsampleFBO("geomFBO", 2);
		singleton->unsampleFBO("combineFBO",0);
		singleton->unbindFBO();
		singleton->unbindShader();

		

		newZoom = std::max(1.0f,singleton->cameraZoom);
		singleton->drawFBO("resultFBO", 0, newZoom );

		
		
	}
GameWorld::~ GameWorld ()
                     {


		threadpool.stopAll();

		int i;

		for (i = 0; i < iVolumeSize; i++) {
			if (worldData[i]) {
				delete worldData[i];
			}
			
		}
		
	}
#undef LZZ_INLINE
 



Singleton* singleton;


void processSpecialKeys(int key, int x, int y) {
    singleton->processSpecialKeys(key,x,y);
}
void processKey(unsigned char key, int x, int y, bool isPressed) {
    singleton->processKey(key,x,y,isPressed);
}
void keyboardUp(unsigned char key, int x, int y) {
    singleton->keyboardUp(key,x,y);
}
void keyboardDown(unsigned char key, int x, int y) {
    singleton->keyboardDown(key,x,y);
}
void mouseMovementWithButton(int x, int y) {
    singleton->mouseMove(x,y);
}
void mouseMovementWithoutButton(int x, int y) {
    singleton->mouseMove(x,y);
}
void mouseClick(int button, int state, int x, int y) {
    singleton->mouseClick(button,state,x,y);
}

void display(void) {
    singleton->display();
}

void reshape (int w, int h) {
    singleton->reshape(w,h);
}
void idleFunc(void) {
    singleton->display();
}

int MAX_CONSOLE_LINES = 500;

void RedirectIOToConsole()

{

    int hConHandle;

    long lStdHandle;

    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    FILE *fp;

    // allocate a console for this app

    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),

                               &coninfo);

    coninfo.dwSize.Y = MAX_CONSOLE_LINES;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),

                               coninfo.dwSize);

    // redirect unbuffered STDOUT to the console

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stdout = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );

    // redirect unbuffered STDIN to the console

    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "r" );

    *stdin = *fp;

    setvbuf( stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console

    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stderr = *fp;

    setvbuf( stderr, NULL, _IONBF, 0 );

    std::ios::sync_with_stdio();

}




WebSocketServer myWebsocketServer;


int main(int argc, char* argv[])
{

    int winWidth;
    int winHeight;
    int scaleFactor;
    int resMode = 0;

    switch (resMode) {
        case 0:
            winWidth = 1024;
            winHeight = 1024;
            scaleFactor = 1;
        break;

        case 1:
            winWidth = 1024;
            winHeight = 512;
            scaleFactor = 1;
        break;

        case 2:
            winWidth = 2048;
            winHeight = 1024;
            scaleFactor = 2;
        break;

        case 3:
            winWidth = 2048;
            winHeight = 1280;
            scaleFactor = 1;
        break;
    }


    RedirectIOToConsole();

    glutInit(&argc, argv);


    //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);  //GLUT_SINGLE

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(140, 200);
    glutCreateWindow("VoxelQuest");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        doTrace("There was an error with GLEW");
    }
    else {
        doTrace("GLEW_OK");
    }

    
    ////////////
    singleton = new Singleton();
    singleton->init(winWidth, winHeight, scaleFactor, &myWebsocketServer);
    

    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMovementWithoutButton);
    glutMotionFunc(mouseMovementWithButton);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(processSpecialKeys);
    

    myWebsocketServer.run(argc, argv);




    

    return 0;
} 
