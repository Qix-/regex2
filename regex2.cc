#include <regex>
#include <string>
#include <iostream>

#include "nan.h"

using namespace Nan;
using namespace std;

// see http://stackoverflow.com/a/38456243/510036
#ifndef CPLUSPLUS17
#	if __cplusplus >= 201406L
#		define CPLUSPLUS17 1
#	endif
#endif

class RegExp2 : public ObjectWrap {
public:
	static NAN_MODULE_INIT(Init) {
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
		tpl->SetClassName(Nan::New("RegExp2").ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(4);

		SetPrototypeMethod(tpl, "toString", ToString);

		constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
		Set(target, Nan::New("RegExp2").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
	}

private:
	explicit RegExp2(const string &pattern, const string &modes)
			: rxp(
#ifdef CPLUSPLUS17
				pattern,
#else
				ModeHasFlag(modes, 'm') ? FixPattern(pattern) : pattern,
#endif
				ParseModes(modes))
			, global(ModeHasFlag(modes, 'g'))
			, source(CleanSource(pattern))
			, modes(CleanModes(modes)) {
	}

	virtual ~RegExp2() {
	}

	static regex::flag_type ParseModes(const string &modes) {
		return regex::ECMAScript
			| regex::optimize
			| (ModeHasFlag(modes, 'i') ? regex::icase : regex::flag_type(0))
#ifdef CPLUSPLUS17
			| (ModeHasFlag(modes, 'm') ? regex::multiline : regex::flag_type(0))
#endif
			;
	}

	static bool ModeHasFlag(const string &modes, char flag) {
		return modes.find(flag) != string::npos;
	}

	static string CleanModes(const string &modes) {
		return string() + (ModeHasFlag(modes, 'g') ? "g" : "") + (ModeHasFlag(modes, 'm') ? "m" : "") + (ModeHasFlag(modes, 'i') ? "i" : "");
	}

	static string CleanSource(const string &str) {
		return str.empty() ? "(?:)" : str;
	}

	static string FixPattern(const string &str) {
		// TODO replace ^ and $ with EOL characters
		return str;
	}

	static NAN_METHOD(New) {
		string pattern = info[0]->IsUndefined() ? string("") : *v8::String::Utf8Value(Nan::To<v8::String>(info[0]).ToLocalChecked());
		string modes = info[1]->IsUndefined() ? string("") : *v8::String::Utf8Value(Nan::To<v8::String>(info[1]).ToLocalChecked());

		try {
			if (info.IsConstructCall()) {
				RegExp2 *obj = new RegExp2(pattern, modes);
				obj->Wrap(info.This());
				info.GetReturnValue().Set(info.This());
			} else {
				const int argc = 2;
				v8::Local<v8::Value> argv[argc] = {info[0], info[1]};
				v8::Local<v8::Function> cons = Nan::New(constructor());
				info.GetReturnValue().Set(cons->NewInstance(argc, argv));
			}
		} catch (const regex_error &e) {
			auto msg = Nan::New("Invalid regular expression /" + pattern + "/" + modes + ": " + e.what()).ToLocalChecked();
			return Nan::ThrowSyntaxError(msg);
		}
	}

	static NAN_METHOD(ToString) {
		RegExp2 *obj = ObjectWrap::Unwrap<RegExp2>(info.Holder());
		info.GetReturnValue().Set(Nan::New("/" + obj->source + "/" + obj->modes).ToLocalChecked());
	}

	static inline Persistent<v8::Function> & constructor() {
		static Persistent<v8::Function> my_constructor;
		return my_constructor;
	}

	regex rxp;
	bool global;
	string source;
	string modes;
};

NODE_MODULE(regex2, RegExp2::Init);
