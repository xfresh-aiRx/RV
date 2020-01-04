
package de.rwth_aachen.techinfo.ltilib;


/**
 * Test class. Do not use.
 */
public class LtiTest {
  public static void main(String[] args) {
    
    Matrix m=new Matrix(2,2);
    Vector v=new Vector(2);
    int i,j;
    
    for (i=0; i<2; i++) {
      for (j=0; j<2; j++) {
        m.setAt(i,j,i+3.0*j);
      }
    }
    for (i=0; i<2; i++) {
      for (j=0; j<2; j++) {
        System.out.println(m.at(i,j));
      }
    }
    
    v=m.getRow(0);
  }
}
