/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * 
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.  
 */ 

 
/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiFastLine_template.h
 * authors ....: Claudia Goenner, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 07.02.2003
 * revisions ..: $Id: ltiFastLine_template.h,v 1.9 2004/05/03 12:20:01 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  // constructors

  template<class T>
  tfastLine<T>::tfastLine() : tline<T>(),uptodate(false) {};

  template<class T>
  tfastLine<T>::tfastLine(const tpoint<T>& theStart, const tpoint<T>& theEnd)
    : tline<T>(theStart,theEnd),uptodate(false) {};

  // --------------------------------
  // distance computation
  // --------------------------------

  template <class T>
  T tfastLine<T>::distanceSqrXPol(const tpoint<T>& c,tpoint<T>& p) const {
    ensureCorrectSlope();
    const float m2p1 = 1.0f/(normSlope*normSlope+1.0f);
    if (invSlope) {
      // inverse slope
      condRoundCastTo((normSlope*(c.y-this->start.y+(normSlope*c.x)) +
                       this->start.x)*
                      m2p1,p.x);
      condRoundCastTo((normSlope*(c.x-this->start.x+(normSlope*this->start.y))+
                       c.y)*
                      m2p1,p.y);
    } else {
      // normal slope
      condRoundCastTo((normSlope*(c.y-this->start.y+
                                  (normSlope*this->start.x)) + c.x)*m2p1,p.x);
      condRoundCastTo((normSlope*(c.x-this->start.x+
                                  (normSlope*c.y)) + this->start.y)*m2p1,p.y);
    }
    return p.distanceSqr(c);
  }

  template <class T>
  T tfastLine<T>::distanceSqrXPol(const tpoint<T>& c) const {
    ensureCorrectSlope();
    const T m2p1 = static_cast<T>(1.0f/(normSlope*normSlope+1.0f));
    if (invSlope) {
      const T tmp = static_cast<T>((normSlope*(c.y-this->start.y)+
                                    (this->start.x-c.x)));
      T res;
      condRoundCastTo(tmp*tmp*m2p1,res);
      return res;
    } else {
      const T tmp = static_cast<T>((normSlope*(this->start.x-c.x)+
                                    (c.x-this->start.y)));
      T res;
      condRoundCastTo(tmp*tmp*m2p1,res);
      return res;
    }
  }


  // ---------------------------------------------
  // intersections
  // ---------------------------------------------

  template <class T>
  //bool tfastLine<T>::intersect(const tfastLine<T>& other, 
  bool tfastLine<T>::getIntersectionPointXPol(const tfastLine<T>& other,
                               tpoint<T>& p, 
                               bool& onThisLine, 
                               bool& onOtherLine,
                               bool& colinear) const {
    
    return tline<T>::getIntersectionPointXPol(other,p,onThisLine,onOtherLine,colinear);

    // followin implementation should be faster than the default line one,
    // but is still not enough optimized

#   if 0
    ensureCorrectSlope();

    // due to the template argument tpoint<U>, this method must be here
    // otherwise MS Visual C++ won't compile this file!
    colinear = false; 
    
    bool parallel = false;
    // just one copy for the whole history of this class.
    // this is computed just once! (it's static)
    static const float epsilon = sqrt(std::numeric_limits<float>::epsilon());

    float mt,mo;
    const bool nmt(getNormalizedSlope(mt));
    const bool nmo(other.getNormalizedSlope(mo));

    // let's assume that the slope at very small changes is direct proportional
    // to the angle changes, so we use slope to check for parallelism.

    if (nmt==nmo) {
      // both slopes have the same 2D subspace
      if (absdiff(mt,mo)<epsilon) {
        parallel=true;
        // lines are parallel, now we need to test for colinearity
        if (nmt) {
          const float xdiff(static_cast<float>(end.x - other.end.x));
          colinear=(static_cast<float>(end.y - xdiff*mt) ==
                    static_cast<float>(other.end.y));
        } else {
          const float ydiff(static_cast<float>(end.y - other.end.y));
          colinear=(static_cast<float>(end.x - ydiff*mt) == 
                    static_cast<float>(other.end.x));
        }
      } else {
        // lines not parallel
        if (nmt) {
          condRoundCastTo((mt*start.x - mo*other.start.x + 
                           other.start.y - start.y)/(mt-mo),p.x);
          condRoundCastTo(start.y + (p.x-start.x)*mt,p.y);
        } else {
          condRoundCastTo((mt*start.y - mo*other.start.y + 
                           other.start.x - start.x)/(mt-mo),p.y);
          condRoundCastTo(start.x + (p.y-start.y)*mt,p.x);
        }
      }

    } else {
      static const float threshold = 1.0f-(epsilon);
      if ( (abs(mt) > threshold) &&
           (abs(mo) > threshold) &&
           (abs(mt-mo) < 1.0f) ) {
        parallel=true;
        // if colinear here, the slope is approximately 1.0
        const float xdiff(static_cast<float>(end.x - other.end.x));
        colinear=(static_cast<float>(end.y - xdiff*mt) == 
                  static_cast<float>(other.end.y));
      } else {
        // lines not parallel
        if (nmt) {
          condRoundCastTo((mo*(other.start.y-start.y+start.x*mt)-
                           other.start.x)/(mo*mt-1),p.x);
          condRoundCastTo(start.y + (p.x-start.x)*mt,p.y);
        } else {
          condRoundCastTo((mo*(other.start.x-start.x+start.y*mt)-
                           other.start.y)/(mo*mt-1),p.y);
          condRoundCastTo(start.x + (p.y-start.y)*mt,p.x);
        }
      }
    }
    
    if (parallel) {
      if (colinear) {
        const tpoint<T> d1(getDelta());

        // lines are colinear, but do they overlap? in latter case they
        // intersect!
        if (d1.x > epsilon) {
          return (onThisLine = onOtherLine =
                  (((start.x <= other.start.x) && (end.x >= other.start.x)) ||
                   ((start.x <= other.end.x)   && (end.x >= other.end.x))));
        } else if (d1.x < -epsilon) {
          return (onThisLine = onOtherLine =
                  (((end.x <= other.start.x) && (start.x >= other.start.x)) ||
                   ((end.x <= other.end.x)   && (start.x >= other.end.x))));
        } else if (d1.y > epsilon) {
          return (onThisLine = onOtherLine =
                  (((start.y <= other.start.y) && (end.y >= other.start.y)) ||
                   ((start.y <= other.end.y)   && (end.y >= other.end.y))));
        } else {
          return (onThisLine = onOtherLine =
                  (((end.y <= other.start.y) && (start.y >= other.start.y)) ||
                   ((end.y <= other.end.y)   && (start.y >= other.end.y))));
        }
      }
      else {
        onThisLine = onOtherLine = false;
        return false;
      }
    }

    // check where is the intersection point
    
    if (nmt) {
      // in this line?
      if (end.x > start.x) {
        onThisLine=((p.x >= start.x) && (p.x <= end.x));
      } else {
        onThisLine=((p.x >= end.x) && (p.x <= start.x));
      }
    } else {
      // in this line?
      if (end.y > start.y) {
        onThisLine=((p.y >= start.y) && (p.y <= end.y));
      } else {
        onThisLine=((p.y >= end.y) && (p.y <= start.y));
      }
    }    

    if (nmo) {
      // in other line?
      if (other.end.x > other.start.x) {
        onOtherLine=((p.x >= other.start.x) && (p.x <= other.end.x));
      } else {
        onOtherLine=((p.x >=  other.end.x) && (p.x <= other.start.x));
      }
    } else {
      // in other line?
      if (other.end.y > other.start.y) {
        onOtherLine=((p.y >= other.start.y) && (p.y <= other.end.y));
      } else {
        onOtherLine=((p.y >=  other.end.y) && (p.y <= other.start.y));
      }
    }

    return (onThisLine && onOtherLine);
#   endif
  }


  template <class T>
  bool tfastLine<T>::doIntersect(const tfastLine<T>& other) const {

    const tpoint<T> d1(getDelta());
    const tpoint<T> d2(other.getDelta());
    const tpoint<T> d0(other.start-this->start);

    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;

    // auxiliar variables containing information of the intersection point
    const T t1 = d2.x*d0.y - d2.y*d0.x;
    const T t2 = d1.x*d0.y - d1.y*d0.x;

    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det > epsilon) {
      return ( ((t1>=static_cast<T>(0)) && (det >= t1)) &&
               ((t2>=static_cast<T>(0)) && (det >= t2)) );
    } else if (det < -epsilon) {
      return ( ((t1<=static_cast<T>(0)) && (det <= t1)) &&
               ((t2<=static_cast<T>(0)) && (det <= t2)) );
    } else {
      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      if ((abs(t1) <= epsilon) && (abs(t2) <= epsilon)) {
        // lines are colinear, but do they overlap? in latter case they
        // intersect!
        if (d1.x > epsilon) {
          return (((this->start.x <= other.start.x) &&
                   (this->end.x >= other.start.x)) ||
                  ((this->start.x <= other.end.x)   &&
                   (this->end.x >= other.end.x)));
        } else if (d1.x < -epsilon) {
          return (((this->end.x <= other.start.x) && 
                   (this->start.x >= other.start.x)) ||
                  ((this->end.x <= other.end.x)   && 
                   (this->start.x >= other.end.x)));
        } else if (d1.y > epsilon) {
          return (((this->start.y <= other.start.y) && 
                   (this->end.y >= other.start.y)) ||
                  ((this->start.y <= other.end.y)   && 
                   (this->end.y >= other.end.y)));
        } else {
          return (((this->end.y <= other.start.y) && 
                   (this->start.y >= other.start.y)) ||
                  ((this->end.y <= other.end.y)   && 
                   (this->start.y >= other.end.y)));
        }          
      } else {
        return false;
      }

    }

    return false;
  }

  template <class T>
  bool tfastLine<T>::intersect(const trectangle<T>& rect) {
    tfastLine<T> xpol(*this);
    if (xpol.intersectXPol(rect)) {
      
      if (this->start.x > this->end.x) {
        if ((this->end.x > xpol.start.x) || (this->start.x < xpol.end.x)){
          return false;
        }
        if (this->start.x > xpol.start.x) {
          this->start = xpol.start;
        }
        if (this->end.x < xpol.end.x) {
          this->end = xpol.end;
        }
        
      } else {
        if ((this->start.x > xpol.end.x) || (this->end.x < xpol.start.x)) {
          return false;
        }
        if (this->start.x < xpol.start.x) {
          this->start = xpol.start;
        }
        if (this->end.x > xpol.end.x) {
          this->end = xpol.end;
        }
      }

      if (this->start.y > this->end.y) {
        if ((this->end.y > xpol.start.y)  || (this->start.y < xpol.end.y)) {
          return false;
        }
        if (this->start.y > xpol.start.y) {
          this->start = xpol.start;
        }
        if (this->end.y < xpol.end.y) {
          this->end = xpol.end;
        }
        
      } else {
        if ((this->start.y > xpol.end.y) || (this->end.y < xpol.start.y)) {
          return false;
        }
        if (this->start.y < xpol.start.y) {
          this->start = xpol.start;
        }
        if (this->end.y > xpol.end.y) {
          this->end = xpol.end;
        }
      }
      
      return true;
    }

    return false;
  }

  template <class T>
  bool tfastLine<T>::intersectXPol(const trectangle<T>& rect) {
 
    tpoint<T> d1;
    float m;
    bool normSlope;
    getLineAttributes(d1,m,normSlope);

    // is a vertical line?
    if (d1.x == static_cast<T>(0)) {
      if ((this->start.x >= rect.ul.x) && (this->start.x <= rect.br.x)) {
        // keep the line direction
        if (this->start.y>this->end.y) {
          this->start.y=rect.br.y;
          this->end.y  =rect.ul.y;
        } else {
          this->start.y=rect.ul.y;
          this->end.y  =rect.br.y;
        }
        return true;
      } else {
        return false;
      }
    }

    // is an horizontal line?
    if (d1.y == static_cast<T>(0)) {
      if ((this->start.y >= rect.ul.y) && (this->start.y <= rect.br.y)) {
        // keep the line direction
        if (this->start.x>this->end.x) {
          this->start.x=rect.br.x;
          this->end.x  =rect.ul.x;
        } else {
          this->start.x=rect.ul.x;
          this->end.x  =rect.br.x;
        }
        return true;
      } else {
        return false;
      }
    }

    if (!invSlope) {
      // lines between -45 degrees and +45 degrees
      // working with equations y = m*x + b
      const float b(static_cast<float>(this->start.y)-static_cast<float>(m*this->start.x));

      if (this->start.x > this->end.x) {
        this->end.x=rect.ul.x;
        this->start.x=rect.br.x;

        condRoundCastTo(m*this->end.x+b,this->end.y);
        if (this->end.y > rect.br.y) {
          this->end.y=rect.br.y;
          condRoundCastTo((this->end.y-b)/m,this->end.x);
          if ((this->end.x<rect.ul.x) || (this->end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (this->end.y < rect.ul.y) {
          this->end.y=rect.ul.y;
          condRoundCastTo((this->end.y-b)/m,this->end.x);
          if ((this->end.x<rect.ul.x) || (this->end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }

        condRoundCastTo(m*this->start.x+b,this->start.y);
        if (this->start.y > rect.br.y) {
          this->start.y=rect.br.y;
          condRoundCastTo((this->start.y-b)/m,this->start.x);
          if ((this->start.x<rect.ul.x) || (this->start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (this->start.y < rect.ul.y) {
          this->start.y=rect.ul.y;
          condRoundCastTo((this->start.y-b)/m,this->start.x);
          if ((this->start.x<rect.ul.x) || (this->start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }
        
      } else {
        // for start.x < end.x
        this->start.x=rect.ul.x;
        this->end.x=rect.br.x;

        condRoundCastTo(m*this->start.x+b,this->start.y);
        if (this->start.y > rect.br.y) {
          this->start.y=rect.br.y;
          condRoundCastTo((this->start.y-b)/m,this->start.x);
          if ((this->start.x<rect.ul.x) || (this->start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (this->start.y < rect.ul.y) {
          this->start.y=rect.ul.y;
          condRoundCastTo((this->start.y-b)/m,this->start.x);
          if ((this->start.x<rect.ul.x) || (this->start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }

        condRoundCastTo(m*this->end.x+b,this->end.y);
        if (this->end.y > rect.br.y) {
          this->end.y=rect.br.y;
          condRoundCastTo((this->end.y-b)/m,this->end.x);
          if ((this->end.x<rect.ul.x) || (this->end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (this->end.y < rect.ul.y) {
          this->end.y=rect.ul.y;
          condRoundCastTo((this->end.y-b)/m,this->end.x);
          if ((this->end.x<rect.ul.x) || (this->end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }
      }

    } else {
      // lines between 45 degrees and 135 degrees
      // working with equations x = m*y + b
      const float b(static_cast<float>(this->start.x)-
                    static_cast<float>(m*this->start.y));

      if (this->start.y > this->end.y) {
        this->end.y=rect.ul.y;
        this->start.y=rect.br.y;

        condRoundCastTo(m*this->end.y+b,this->end.x);
        if (this->end.x > rect.br.x) {
          this->end.x=rect.br.x;
          condRoundCastTo((this->end.x-b)/m,this->end.y);
          if ((this->end.y<rect.ul.y) || (this->end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (this->end.x < rect.ul.x) {
          this->end.x=rect.ul.x;
          condRoundCastTo((this->end.x-b)/m,this->end.y);
          if ((this->end.y<rect.ul.y) || (this->end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }

        condRoundCastTo(m*this->start.y+b,this->start.x);
        if (this->start.x > rect.br.x) {
          this->start.x=rect.br.x;
          condRoundCastTo((this->start.x-b)/m,this->start.y);
          if ((this->start.y<rect.ul.y) || (this->start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (this->start.x < rect.ul.x) {
          this->start.x=rect.ul.x;
          condRoundCastTo((this->start.x-b)/m,this->start.y);
          if ((this->start.y<rect.ul.y) || (this->start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }
        
      } else {
        // for start.y < end.y
        this->start.y=rect.ul.y;
        this->end.y=rect.br.y;

        condRoundCastTo(m*this->start.y+b,this->start.x);
        if (this->start.x > rect.br.x) {
          this->start.x=rect.br.x;
          condRoundCastTo((this->start.x-b)/m,this->start.y);
          if ((this->start.y<rect.ul.y) || (this->start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (this->start.x < rect.ul.x) {
          this->start.x=rect.ul.x;
          condRoundCastTo((this->start.x-b)/m,this->start.y);
          if ((this->start.y<rect.ul.y) || (this->start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }

        condRoundCastTo(m*this->end.y+b,this->end.x);
        if (this->end.x > rect.br.x) {
          this->end.x=rect.br.x;
          condRoundCastTo((this->end.x-b)/m,this->end.y);
          if ((this->end.y<rect.ul.y) || (this->end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (this->end.x < rect.ul.x) {
          this->end.x=rect.ul.x;
          condRoundCastTo((this->end.x-b)/m,this->end.y);
          if ((this->end.y<rect.ul.y) || (this->end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }
      }
    }

    return true;
  }

  // ---------------------------------------------------
  // Storable interface
  // ---------------------------------------------------

  /**
   * read the vector from the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be read
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,tfastLine<T>& l, const bool complete) {
    bool success(true);

    if (complete) {
      success = success && handler.readBegin();
    }

    tpoint<T> a,b;
    success = success && lti::read(handler,a);
    success = success && handler.readDataSeparator();
    success = success && lti::read(handler,a);
    l.set(a,b);

    if (complete) {
      success = success && handler.readEnd();
    }

    
    return success;
  };
  

}//namespace lti

namespace std {

  template <class T>
  inline istream& operator>>(istream& s,lti::tfastLine<T>& l) {
    char c;
    lti::tpoint<T> a,b;
    s >> c 
      >> a >> c 
      >> b >> c;

    l.set(a,b);
    return s;
  };
}//namespace std

