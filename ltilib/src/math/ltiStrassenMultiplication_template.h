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


/*
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiStrassenMultiplication_template.h
 * authors ....: Xin Gu
 * organization: LTI, RWTH Aachen
 * creation ...: 17.12.00
 * revisions ..: $Id: ltiStrassenMultiplication_template.h,v 1.5 2004/05/03 13:30:10 alvarado Exp $
 * description.:
 * The Program implements the Winograd's variant of Strassen'algorithm
 * The matrix multiplication is represented in the divide-and-conquer form
 * There are 15 matrix additions and 7 matrix multiplications.
 */

//#include "ltiMatrixMultipleOfStrasse.h"

namespace lti{

  //default  constructor
  template<class T>
  strassenMultiplication<T>::strassenMultiplication() {
  }


  //copy construct
  template<class T>
  strassenMultiplication<T>::strassenMultiplication(const strassenMultiplication<T>& other) {
  }

  //destructor
  template<class T>
  strassenMultiplication<T>::~strassenMultiplication() {
  }


  template<class T>
  matrix<T> &strassenMultiplication<T>::multiply(const matrix<T>& one,
                                                 const matrix<T>& other,
                                                 matrix<T> &product){

    //*** warning:->the function is not complete!

    //instanceOf
    strassenMultiplication<T> matrixMultiple;

    //get the dimensions of the matrices
    int m=one.rows();
    int m1=m;   // backup for m;
    int n=other.columns();
    int n1=n;   // backup for n;
    int k=one.columns();
    int a,b,c;  //variables for the recursion truncation   //-multiplication

    a=199;b=1;c=1;

    matrix<T> tmp1,tmp2;
    tmp1=one;
    tmp2=other;
    //std::cout<<"tmp1 : "<<tmp1.rows()<<endl<<tmp1.columns()<<endl;
    //std::cout<<"tmp2 : "<<tmp2.rows()<<endl<<tmp2.columns()<<endl;
    //std::cout<<"k : "<<k<<endl;
    //test if the multiplation is valid?
    if (k!=tmp2.rows()){
      //throw ;
      //std::cout<<"The multiplication of the two matrices is invalid!"<<endl;
      throw incompatibleDimensionsException();
    }

    //return the product if the matrix is small
    //std::cout<<"running"<<endl;
    if ((m<=a)||(n<=b)||(k<=c)) {
      return product.multiply(tmp1,tmp2);
    }

    //add a zero row-vector(0) and (or)column-vector(0) to  the matrices if
    // they are not even.
    if (isodd(m)) {
      //add a row-vector(0) to src
      tmp1.resize(++m,k,T(0),true,true);
    }
    if (isodd(n)) {
      //add a column-vector(0) to dest
      tmp2.resize(k,++n,T(0),true,true);
    }
    if (isodd(k)) {
      //add a column-vector(0) to src
      tmp1.resize(m,++k,T(0),true,true);
      //add a row-vector(0) to dest
      tmp2.resize(k,n,T(0),true,true);
    }

    if (!multiplyHighlyRectangularMatrix(tmp1,tmp2,product)) {

      // std::cout<<"tmp1 :"<<tmp1<<endl;
      // std::cout<<"tmp2 :"<<tmp2<<endl<<endl;

      // std::cout<<"m: "<<m<<endl;
      //std::cout<<"k: "<<k<<endl;
      //std::cout<<"n: "<<n<<endl;
      matrix<T> A[2][2];
      matrix<T> B[2][2];

      //initializing the matrices A,B;

      A[0][0].resize(m/2,k/2);
      A[0][1].resize(m/2,k/2);
      A[1][0].resize(m/2,k/2);
      A[1][1].resize(m/2,k/2);

      B[0][0].resize(k/2,n/2);
      B[0][1].resize(k/2,n/2);
      B[1][0].resize(k/2,n/2);
      B[1][1].resize(k/2,n/2);

      A[0][0].fill(tmp1,0,m/2-1,0,k/2-1,0,0);
      A[0][1].fill(tmp1,0,m/2-1,0,k/2-1,0,k/2);
      A[1][0].fill(tmp1,0,m/2-1,0,k/2-1,m/2,0);
      A[1][1].fill(tmp1,0,m/2-1,0,k/2-1,m/2,k/2);

      B[0][0].fill(tmp2,0,k/2-1,0,n/2-1,0,0);
      B[0][1].fill(tmp2,0,k/2-1,0,n/2-1,0,n/2);
      B[1][0].fill(tmp2,0,k/2-1,0,n/2-1,k/2,0);
      B[1][1].fill(tmp2,0,k/2-1,0,n/2-1,k/2,n/2);

      /*
        //Implementation -> Strassen1
        //addition
        matrix<T> tmp[8];
        tmp[0].add(A[1][0],A[1][1]);//S1
        tmp[1].subtract(tmp[0],A[0][0]);//S2
        tmp[2].subtract(A[0][0],A[1][0]);//S3
        tmp[3].subtract(A[0][1],tmp[1]);//S4

        tmp[4].subtract(B[0][1],B[0][0]);//T1
        tmp[5].subtract(B[1][1],tmp[4]);//T2
        tmp[6].subtract(B[1][1],B[0][1]);//T3
        tmp[7].subtract(B[1][0],tmp[5]);//T4

        //"*" will be replaced by the multiplication under the strassen's algorithm
        matrix<T> temp[7];
        temp[0]=matrixMultiple.multiply(A[0][0],B[0][0]);//P1
        temp[1]=matrixMultiple.multiply(A[0][1],B[1][0]);//P2
        temp[2]=matrixMultiple.multiply(tmp[0],tmp[4]);//P3
        temp[3]=matrixMultiple.multiply(tmp[1],tmp[5]);//P4
        temp[4]=matrixMultiple.multiply(tmp[2],tmp[6]);//P5
        temp[5]=matrixMultiple.multiply(tmp[3],B[1][1]);//P6
        temp[6]=matrixMultiple.multiply(A[1][1],tmp[7]);//P7

        matrix<T> U[7];
        U[0].add(temp[0],temp[1]);//C11
        U[1].add(temp[0],temp[3]);
        U[2].add(U[1],temp[4]);
        U[3].add(U[2],temp[6]);//C21
        U[4].add(U[2],temp[2]);//C22
        U[5].add(U[1],temp[2]);
        U[6].add(U[5],temp[5]);//C12

        //merge the 4  matrices into the matrix c
        product.resize(m,n,T(0),false,false);
        product.fill(U[0],0,m/2-1,0,n/2-1,0,0);//merge the C11 into matrix C
        product.fill(U[3],m/2,m-1,0,n/2-1,0,0);//merge the C21 into matrix C
        product.fill(U[6],0,m/2-1,n/2,n-1,0,0);//merge the C12 into matrix C
        product.fill(U[4],m/2,m-1,n/2,n-1,0,0);//merge the C22 into matrix C
      */

      //Implementation-> Strassen2
      matrix<T> R[3];
      matrix<T> C[2][2];
      R[0].add(A[1][0],A[1][1]);
      R[1].subtract(B[0][1],B[0][0]);
      //R[2]=matrixMultiple.multiply(R[0],R[1]);
      R[2]=matrixMultiple.multiply(R[0],R[1],R[2]);
      C[0][1]=R[2];//b*C12+@P3
      C[1][1]=R[2];//b*C22+@P3
      R[0].subtract(A[0][0]);
      R[1].subtract(B[1][1],R[1]);
      //R[2]=matrixMultiple.multiply(A[0][0],B[0][0]);
      matrixMultiple.multiply(A[0][0],B[0][0],R[2]);
      C[0][0]=R[2];//b*C11+@P1
      matrixMultiple.multiply(R[0],R[1],tmp1);
      R[2].add(tmp1);
      matrixMultiple.multiply(A[0][1],B[1][0],tmp1);
      C[0][0].add(tmp1);//b*C11+@P2
      R[0].subtract(A[0][1],R[0]);
      R[1].subtract(B[1][0],R[1]);
      matrixMultiple.multiply(R[0],B[1][1],tmp1);
      C[0][1].add(tmp1);
      C[0][1].add(R[2]);
      matrixMultiple.multiply(A[1][1],R[1],C[1][0]);//b*C21+@P7
      R[0].subtract(A[0][0],A[1][0]);
      R[1].subtract(B[1][1],B[0][1]);
      matrixMultiple.multiply(R[0],R[1],tmp1);
      R[2].add(tmp1);
      C[1][0].add(R[2]);
      C[1][1].add(R[2]);

      //merge the 4 sub matices into a matrix C
      product.resize(m,n,T(0),false,false);
      product.fill(C[0][0],0,m/2-1,0,n/2-1,0,0);
      product.fill(C[1][0],m/2,m-1,0,n/2-1,0,0);
      product.fill(C[0][1],0,m/2-1,n/2,n-1,0,0);
      product.fill(C[1][1],m/2,m-1,n/2,n-1,0,0);

      //std::cout<<"C11 :"<<C[0][0]<<endl;
      //std::cout<<"C21 :"<<C[1][0]<<endl;
      //std::cout<<"C12 :"<<C[0][1]<<endl;
      //std::cout<<"C22 :"<<C[1][1]<<endl;
      //reduce the result from the padded row-vector(0)
      product.resize(m1,n1,T(0),true,true);

      //reduce the result from the padded column-vector(0)
      //product.resize(m,n1,T(0),true,true);
    }
    return product;
  }


  template<class T>
  bool strassenMultiplication<T>::multiplyHighlyRectangularMatrix(const matrix<T>& one,
                                                                  const matrix<T>& other,
                                                                  matrix<T>& product) {

    //get the nums of columns and rows
    int row1=one.rows();
    int column1=one.columns();
    int column2=other.columns();

    matrix<T> A1,A2,B1,B2;
    matrix<T> tmp;
    strassenMultiplication<T> multiple;

    product.resize(row1,column2);

    //test,if the matrix is a highly rectangular matrix

    //Lean A and wide B
    if ((row1/column1 >= 4)&&(column2/column1 >= 4)){

      //devide matrix one into 2 matrices
      A1.resize( row1/2  , column1 );
      A2.resize( row1/2  , column1 );
      A1.fill(one,0,row1/2-1,0,column1-1,0,0);
      A2.fill(one,0,row1/2-1,0,column1-1,row1/2,0);

      //devide matrix other into 2 matrices
      B1.resize( column1  , column2 /2);
      B2.resize( column1  , column2 /2);
      B1.fill(other,0,column1-1,0,column2/2-1,0,0);
      B2.fill(other,0,column1-1,0,column2/2-1,0,column2/2);

      multiple.multiply(A1,B1,tmp);
      product.fill(tmp,0,row1/2-1,0,column2/2-1,0,0);
      multiple.multiply(A1,B2,tmp);
      product.fill(tmp,0,row1/2-1,column2/2,column2-1,0,0);
      multiple.multiply(A2,B1,tmp);
      product.fill(tmp,row1/2,row1-1,0,column2/2-1,0,0);
      multiple.multiply(A2,B2,tmp);
      product.fill(tmp,row1/2,row1-1,column2/2,column2-1,0,0);
      //std::cout<<"return product A"<<endl<<endl;
      return true;
    }

    //Wide A and lean B
    if ((column1/row1 >= 4)&&(column1/column2 >= 4)){

      //devide matrix one into 2 matrices
      A1.resize( row1  , column1 /2);
      A2.resize( row1  , column1 /2);
      A1.fill(one,0,row1-1,0,column1/2-1,0,0);
      A2.fill(one,0,row1-1,0,column1/2-1,0,column1/2);

      //devide matrix other into 2 matrices
      B1.resize( column1 /2 , column2 );
      B2.resize( column1 /2 , column2 );
      B1.fill(other,0,column1/2-1,0,column2-1,0,0);
      B2.fill(other,0,column1/2-1,0,column2-1,column1/2,0);

      multiple.multiply(A1,B1,product);
      multiple.multiply(A2,B2,tmp);
      product.add(tmp);
      //std::cout<<"return product B"<<endl<<endl;
      return true;
    }

    //Wide B or lean A
    if ((column1/row1 >= 4)||(column2/column1 >= 4)){

      //devide matrix other into 2 matrices
      B1.resize( column1  , column2 /2);
      B2.resize( column1  , column2 /2);
      B1.fill(other,0,column1-1,0,column2/2-1,0,0);
      B2.fill(other,0,column1-1,0,column2/2-1,0,column2/2);

      multiple.multiply(one,B1,tmp);
      product.fill(tmp,0,row1-1,0,column2/2-1,0,0);
      multiple.multiply(one,B2,tmp);
      product.fill(tmp,0,row1-1,column2/2,column2-1,0,0);
      //std::cout<<"return product C"<<endl<<endl;
      return true;
    }

    //Wide A or lean B
    if ((column1/column2 >= 4)||(row1/column1 >= 4)){

      //devide matrix one into 2 matrices
      A1.resize( row1/2  , column1 );
      A2.resize( row1/2  , column1 );
      A1.fill(one,0,row1/2-1,0,column1-1,0,0);
      A2.fill(one,0,row1/2-1,0,column1-1,row1/2,0);

      multiple.multiply(A1,other,tmp);
      product.fill(tmp,0,row1/2-1,0,column2-1,0,0);
      multiple.multiply(A2,other,tmp);
      product.fill(tmp,row1/2,row1-1,0,column2-1,0,0);
      //std::cout<<"return product D"<<endl<<endl;
      return true;
    }

    //std::cout<<"return 0"<<endl;
    return false;
  }

  template <class T>
  const char* strassenMultiplication<T>::getTypeName() const {
    return "strassenMultiplication";
  }

  // returns a pointer to a clone of the functor.
  template <class T>
  functor* strassenMultiplication<T>::clone() const {
    return (new strassenMultiplication<T>(*this));
  };


}

