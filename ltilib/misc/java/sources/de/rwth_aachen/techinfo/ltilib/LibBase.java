
package de.rwth_aachen.techinfo.ltilib;

import java.util.*;
import java.io.*;
import java.net.*;
import java.util.jar.*;


/** This class serves as a base class for all interface classes to the
 *  LTI library. It contains data that are needed for bookkeeping on
 *  the C++ side. 
 * @version $Id: LibBase.java,v 1.4 2003/03/06 18:01:36 wickel Exp $
 */

public class LibBase {
  /** Holder variable for the native C++ object reference. This variable
   *  contains the address of the C++ object which is created by the
   *  construction of an instance of a subclass of this class. 
   */
  protected long nativeObject;
  /** Flag which determines if the C++ object should be destroyed together
   *  with the corresponding Java gateway object. Normally, it should
   *  always be true. However, there are thinkable cases when a C++ object
   *  could have several Java gateway objects.
   */
  protected boolean destroy;

  private static Properties props=null;

  static private Properties getProperties() {
    if (props == null) {
      try {
        InputStream s;

        // System.out.println("1");
        URL u=ClassLoader.getSystemClassLoader().getResource("LTIlib.properties");
        // System.out.println("2");
        if (u != null) {
          // System.out.println("Pref: "+u.getClass().getName());
          String fname=u.toString();
          // System.out.println("3");
          //System.err.println("Pref: "+fname);
          File propFile=new File(u.getPath());
          // System.out.println("4");
          props=new Properties();
          if (u.getProtocol().equals("jar")) {
            // We come from a jar file
            // System.out.println("J");
            JarURLConnection conn=(JarURLConnection)u.openConnection();
            JarEntry entry=conn.getJarEntry();
            JarFile jfile=conn.getJarFile();
            s=jfile.getInputStream(entry);
          } else {
            // System.out.println("5");
            s=new FileInputStream(propFile);
          }
          props.load(s);
          s.close();
          // System.out.println("6");
        } else {
          props=new Properties();
        }
      } catch (java.io.IOException x) {
        // System.out.println(x);
        props=new Properties();
      }
    }
    return props;
  }

  static protected void loadNativeLibrary() {
    //System.out.println("Loading LTI-JNI");
    String tmp=getProperties().getProperty("libName");
    if (tmp == null || tmp.length() == 0) {
      System.loadLibrary("ltiInterface");
      //System.out.println("Loading default");
    } else {
      System.loadLibrary(tmp);
      //System.out.println("Loading "+tmp);
    }
    //System.out.println("Done loading LTI-JNI");
  }

  public long getNativeObject() {
    return nativeObject;
  }

  static {
    loadNativeLibrary();
  }

}

