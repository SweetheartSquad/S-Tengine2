#pragma once

#include <string>

/**
 * Contains the standard Robert Penner easing equations (ported from the ones we made for the Haxe library)
 * @author Sean
 */
class Easing{
public:
	enum Type{
		kLINEAR,
		kEASE_IN_QUAD,
		kEASE_OUT_QUAD,
		kEASE_IN_OUT_QUAD,
		kEASE_IN_CUBIC,
		kEASE_OUT_CUBIC,
		kEASE_IN_OUT_CUBIC,
		kEASE_IN_QUART,
		kEASE_OUT_QUART,
		kEASE_IN_OUT_QUART,
		kEASE_IN_QUINT,
		kEASE_OUT_QUINT,
		kEASE_IN_OUT_QUINT,
		kEASE_IN_SINE,
		kEASE_OUT_SINE,
		kEASE_IN_OUT_SINE,
		kEASE_IN_EXPO,
		kEASE_OUT_EXPO,
		kEASE_IN_OUT_EXPO,
		kEASE_IN_CIRC,
		kEASE_OUT_CIRC,
		kEASE_IN_OUT_CIRC,
		kEASE_IN_ELASTIC,
		kEASE_OUT_ELASTIC,
		kEASE_IN_OUT_ELASTIC,
		kEASE_IN_BOUNCE,
		kEASE_OUT_BOUNCE,
		kEASE_IN_OUT_BOUNCE,
		kEASE_IN_BACK,
		kEASE_OUT_BACK,
		kEASE_IN_OUT_BACK,
	};
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float call(Type type, float t, float b, float c, float d){
		switch(type) {
			case kLINEAR: return linear(t,b,c,d);
			case kEASE_IN_QUAD:			return easeInQuad(t,b,c,d);
			case kEASE_OUT_QUAD:		return easeOutQuad(t,b,c,d);
			case kEASE_IN_OUT_QUAD:		return easeInOutQuad(t,b,c,d);
			case kEASE_IN_CUBIC:		return easeInCubic(t,b,c,d);
			case kEASE_OUT_CUBIC:		return easeOutCubic(t,b,c,d);
			case kEASE_IN_OUT_CUBIC:	return easeInOutCubic(t,b,c,d);
			case kEASE_IN_QUART:		return easeInQuart(t,b,c,d);
			case kEASE_OUT_QUART:		return easeOutQuart(t,b,c,d);
			case kEASE_IN_OUT_QUART:	return easeInOutQuart(t,b,c,d);
			case kEASE_IN_QUINT:		return easeInQuint(t,b,c,d);
			case kEASE_OUT_QUINT:		return easeOutQuint(t,b,c,d);
			case kEASE_IN_OUT_QUINT:	return easeInOutQuint(t,b,c,d);
			case kEASE_IN_SINE:			return easeInSine(t,b,c,d);
			case kEASE_OUT_SINE:		return easeOutSine(t,b,c,d);
			case kEASE_IN_OUT_SINE:		return easeInOutSine(t,b,c,d);
			case kEASE_IN_EXPO:			return easeInExpo(t,b,c,d);
			case kEASE_OUT_EXPO:		return easeOutExpo(t,b,c,d);
			case kEASE_IN_OUT_EXPO:		return easeInOutExpo(t,b,c,d);
			case kEASE_IN_CIRC:			return easeInCirc(t,b,c,d);
			case kEASE_OUT_CIRC:		return easeOutCirc(t,b,c,d);
			case kEASE_IN_OUT_CIRC:		return easeInOutCirc(t,b,c,d);
			case kEASE_IN_ELASTIC:		return easeInElastic(t,b,c,d);
			case kEASE_OUT_ELASTIC:		return easeOutElastic(t,b,c,d);
			case kEASE_IN_OUT_ELASTIC:	return easeInOutElastic(t,b,c,d);
			case kEASE_IN_BOUNCE:		return easeInBounce(t,b,c,d);
			case kEASE_OUT_BOUNCE:		return easeOutBounce(t,b,c,d);
			case kEASE_IN_OUT_BOUNCE:	return easeInOutBounce(t,b,c,d);
			case kEASE_IN_BACK:			return easeInBack(t,b,c,d);
			case kEASE_OUT_BACK:		return easeOutBack(t,b,c,d);
			case kEASE_IN_OUT_BACK:		return easeInOutBack(t,b,c,d);
		}
		return 0;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float linear(float t, float b, float c, float d){
		return c * t / d + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInQuad(float t, float b, float c, float d){
		return c*(t/=d)*t + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutQuad(float t, float b, float c, float d){
		return -c *(t/=d)*(t-2) + b;
	}
	
	/**
	 * This one might not be right
	 * Currently:	return -c/2 * (((t-2)*(--t)) - 1) + b;
	 * Could be:	return -c/2 * (((--t)*(t-2)) - 1) + b;
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutQuad(float t, float b, float c, float d){
		if ((t /= d / 2) < 1) {
			return ((c/2)*(t*t)) + b;
		}
		return -c/2 * (((t-2)*(--t)) - 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInCubic(float t, float b, float c, float d){
		return c*(t/=d)*t*t + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutCubic(float t, float b, float c, float d){
		return c*((t=t/d-1)*t*t + 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutCubic(float t, float b, float c, float d){
		if ((t /= d / 2) < 1) {
			return c/2*t*t*t + b;
		}
		return c/2*((t-=2)*t*t + 2) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInQuart(float t, float b, float c, float d){
		return c*(t/=d)*t*t*t + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutQuart(float t, float b, float c, float d){
		return -c * ((t=t/d-1)*t*t*t - 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutQuart(float t, float b, float c, float d){
		if ((t /= d / 2) < 1) {
			return c/2*t*t*t*t + b;
		}
		return -c/2 * ((t-=2)*t*t*t - 2) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInQuint(float t, float b, float c, float d){
		return c*(t/=d)*t*t*t*t + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutQuint(float t, float b, float c, float d){
		return c*((t=t/d-1)*t*t*t*t + 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutQuint(float t, float b, float c, float d){
		if ((t /= d / 2) < 1) {
			return c/2*t*t*t*t*t + b;
		}
		return c/2*((t-=2)*t*t*t*t + 2) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInSine(float t, float b, float c, float d){
		return -c * cosf(t/d * ((3.141592f)/2)) + c + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutSine(float t, float b, float c, float d){
		return c * sinf(t/d * ((3.141592f)/2)) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutSine(float t, float b, float c, float d){
		return -c/2 * (cos((3.141592f)*t/d) - 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInExpo(float t, float b, float c, float d){
		return (t==0) ? b : c * powf(2, 10 * (t/d - 1)) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutExpo(float t, float b, float c, float d){
		return (t==d) ? b+c : c * (-powf(2, -10 * t/d) + 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutExpo(float t, float b, float c, float d){
		if (t == 0) {
			return b;
		}
		if (t == d) {
			return b+c;
		}
		if ((t /= d / 2) < 1) {
			return c/2 * powf(2, 10 * (t - 1)) + b;
		}
		return c/2 * (-powf(2, -10 * --t) + 2) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInCirc(float t, float b, float c, float d){
		return -c * (sqrtf(1 - (t/=d)*t) - 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutCirc(float t, float b, float c, float d){
		return c * sqrtf(1 - (t=t/d-1)*t) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutCirc(float t, float b, float c, float d){
		if ((t /= d / 2) < 1) {
			return -c/2 * (sqrtf(1 - t*t) - 1) + b;
		}
		return c/2 * (sqrtf(1 - (t-=2)*t) + 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	p	Period (defaults to d*0.3 if -1)
	 * @param	a	Amplitude (defaults to c if -1)
	 * @param	s	Tweak parameter (defaults to p/4 if -1)
	 * @return	Eased value
	 */
	static float easeInElastic(float t, float b, float c, float d, float p = -1, float a = -1, float s = -1){
		if (t == 0) {
			return b;
		}
		if ((t /= d) == 1) {
			return b+c;  
		}
		if (p == -1) {
			p = d * 0.3f;
		}
		if (a == -1) {
			a = c;
		}else if (a < abs(c)) {
			a = c;
		}
		if (s == -1) {
			s = p / 4;
		}
		float postFix = a*powf(2, 10*(t-=1));
		return -(postFix * sinf((t*d-s)*(2*(3.141592f)) / p )) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	p	Period (defaults to d*0.3 if -1)
	 * @param	a	Amplitude (defaults to c if -1)
	 * @param	s	Tweak parameter (defaults to p/4 if -1)
	 * @return	Eased value
	 */
	static float easeOutElastic(float t, float b, float c, float d, float p = -1, float a = -1, float s = -1){
		if (t == 0) {
			return b;
		}
		if ((t /= d) == 1) {
			return b+c;  
		}
		if (p == -1) {
			p = d * 0.3f;
		}
		if (a == -1) {
			a = c;
		}else if (a < abs(c)) {
			a = c;
		}
		if (s == -1) {
			s = p / 4;
		}
		return (a*powf(2, -10*t) * sinf( (t*d-s)*(2*(3.141592f))/p ) + c + b);	
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	p	Period (defaults to d*0.3 if -1)
	 * @param	a	Amplitude (defaults to c if -1)
	 * @param	s	Tweak parameter (defaults to p/4 if -1)
	 * @return	Eased value
	 */
	static float easeInOutElastic(float t, float b, float c, float d, float p = -1, float a = -1, float s = -1){
		if (t == 0) {
			return b;
		}
		if ((t/=d/2)==2) {
			return b+c;  
		}
		if (p == -1) {
			p = d*(0.3f*1.5f);
		}
		if (a == -1) {
			a = c;
		}else if (a < abs(c)) {
			a = c;
		}
		if (s == -1) {
			s = p / 4;
		}
		
		if (t < 1) {
			float postFix =a*powf(2,10*(t-=1));
			return -0.5f*(postFix* sinf( (t*d-s)*(2*(3.141592f))/p )) + b;
		} 
		float postFix =  a*powf(2,-10*(t-=1));
		return postFix * sinf( (t*d-s)*(2*(3.141592f))/p )*0.5f + c + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	s	Tweak parameter
	 * @return	Eased value
	 */
	static float easeInBack(float t, float b, float c, float d, float s = 1.70158f){
		float postFix = t/=d;
		return c*(postFix)*t*((s+1)*t - s) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	s	Tweak parameter
	 * @return	Eased value
	 */
	static float easeOutBack(float t, float b, float c, float d, float s = 1.70158f){
		return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @param	s	Tweak parameter
	 * @return	Eased value
	 */
	static float easeInOutBack(float t, float b, float c, float d, float s = 1.70158f){
		if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
		float postFix = t-=2;
		return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
	}
	
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInBounce(float t, float b, float c, float d){
		return c - easeOutBounce (d-t, 0, c, d) + b;
	}
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeOutBounce(float t, float b, float c, float d){
		if ((t/=d) < (1/2.75f)) {
			return c*(7.5625f*t*t) + b;
		} else if (t < (2/2.75f)) {
			float postFix = t-=(1.5f/2.75f);
			return c*(7.5625f*(postFix)*t + 0.75f) + b;
		} else if (t < (2.5f/2.75f)) {
			float postFix = t-=(2.25f/2.75f);
			return c*(7.5625f*(postFix)*t + 0.9375f) + b;
		} else {
			float postFix = t-=(2.625f/2.75f);
			return c*(7.5625f*(postFix)*t + 0.984375f) + b;
		}
	}
	/**
	 * @param	t	Current time
	 * @param	b	Start value
	 * @param	c	Change in value (target - start value)
	 * @param	d	Duration
	 * @return	Eased value
	 */
	static float easeInOutBounce(float t, float b, float c, float d){
		if (t < d/2) return easeInBounce(t*2, 0, c, d) * .5f + b;
		return easeOutBounce(t*2-d, 0, c, d) * .5f + c*.5f + b;
	}
};

/*
 *
 * TERMS OF USE - EASING EQUATIONS
 * 
 * Open source under the BSD License. 
 * 
 * Copyright Â© 2001 Robert Penner
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of 
 * conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list 
 * of conditions and the following disclaimer in the documentation and/or other materials 
 * provided with the distribution.
 * 
 * Neither the name of the author nor the names of contributors may be used to endorse 
 * or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 */