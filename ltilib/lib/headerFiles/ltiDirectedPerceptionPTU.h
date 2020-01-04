/*
 * Copyright (C) 1998 - 2004
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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiDirectedPerceptionPTU.h
 * authors ....: Christian Hein, Florian Bley
 * organization: LTI, RWTH Aachen
 * creation ...: 17.12.2004
 * revisions ..: $Id: ltiDirectedPerceptionPTU.h,v 1.2 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_DIRECTED_PERCEPTION_P_T_U_H_
#define _LTI_DIRECTED_PERCEPTION_P_T_U_H_


#include "ltiPanTiltUnit.h"
#include "ltiSerial.h"



namespace lti {
  /**
   * This class performs handling of the pan-tilt unit PTU-D46-17 from the manufactor DirectedPerception.<br>
   * <TABLE>
   * <TR>
   * <TD COLSPAN="3">Base data (taken from the specifications)</TD>
   * </TR>
   * <TR>
   * <TD ALIGN="LEFT">Pan range</TD> <TD ALIGN="RIGHT">-159 degrees</TD> <TD>+159 degrees</TD>
   * </TR>
   * <TR>
   * <TD ALIGN="LEFT">Tilt Range</TR> <TD ALIGN="Right">-47 degrees</TD> <TD>+31 degrees</TD>
   * </TR>
   * <TR>
   * <TD ALIGN="LEFT">Max unloaded speed</TD> <TD COLSPAN="2" ALIGN="RIGHT">300 degrees/second</TD>
   * </TR>
   * <TR>
   * <TD ALIGN="LEFT">Resolution</TD> <TD COLSPAN="2" ALIGN="RIGHT">3.086' = 0.051428 degrees/position</TD>
   * </TR>
   * </TABLE>
   * <br>
   * <b>Initializing</b><br>
   * Before you start sending any commands, the pan-tilt unit (PTU) has to be initialized. This happens
   * by using directedPerceptionPTU::initialize(). If this method has not been successfully
   * executed, all methods working on the PTU will not do and return a false with
   * an adequate report in the status string. The routine tests the communication between the
   * serial port and the PTU, sets pan/tilt position to 0,0, acceleration and speed to default values.
   * They should deal with the most mounted loads.<br>
   * <br>
   * 
   * <b>Execution modes</b><br>
   * As you can see from the corresponding methods, the pan-tilt unit provides two execution functions:
   * slaved and immidiate. The standard mode is immidiate. In immidiate mode, position commands are
   * excuted at once, when they have been sent to the unit. In slaved mode, position commands are 
   * not executed before an await command was send. That means in fact not before 
   * directedPerceptionPTU::awaitPosCommandCompletion() is applied. This realizes that pan and tilt 
   * movement act more or less synchronously. An adequate example in angle format degrees might be:
   * <br>
   * \code
   * lti::directedPerceptionPTU ptu;
   * ptu.setExecMode(directedPerceptionPTU::parameters::slaved);
   * ptu.setPan(-90.f);
   * ptu.setTilt(30.f);
   * ptu.awaitPosCommandCompletion();
   * \endcode
   * <br>
   * If you give more than one pan and tilt combination only the last one will be applied, because
   * the earlier ones are overwritten immediately.<br>
   * 
   * <b>Speeds and Accelerations</b><br>
   * Note that minimum/maximum pan and tilt speeds/accelerations are more less experimental. So
   * some speed and acceleration combinations could not be legal. As in speed and acceleration setting 
   * it is not checked, whether the pan-tilt unit took over the changes correctly, it could happen, 
   * that a setting function returns true, although the PTU rejected the given values. If you have
   * the feeling that some settings were not applied, try values, which have a larger distance to
   * the max. or min. borders. In most cases this will not occur, since it is not necessary to set
   * extreme speed and acceleration values for common applications. Also the max/min speeds and
   * accelerations are already limited.<br>
   * If you like to experiment with the pan-tilt unit to test it, use a terminal programm (e.g.
   * minicom under linux) to set up communication between the pc and the device. Work with 9600 
   * baud, 1 start bit, 8 data bits, 1 stop bit, no parity. The instructions can be taken from
   * the manual which you can find under <a href="http://www.directedperception.com">www.directedPerception.com</a> <br>
   *  
   * <b>Relative positions</b><br>
   * As announced in some methods, relative positioning provides the danger of huge divergences
   * between the real and the internal positions. This can occur, if a big amount of relative
   * position commands are set after each other very fast, without a sufficent break between them.
   * Then the internal positions are faster updated, than the the pan-tilt could execute the commands.
   * What happens, is the following: E.g. the PTU gets the command to pan relatively with a few degrees.
   * Before that is finished, the next command arrives and tells to pan again relatively with some degrees.
   * Now the position of the new command does not calculate from where the first would end in a 
   * few seconds, but from where it is currently located. Thus, errors accumulate over time.
   * That results in an decreasing driving interval . This could be avoided, if a long enoug sleep command is 
   * applied between position setting or working with the await and isIdle methods as shown in the example.
   * If a not wanted difference has been recognized the internal values could be updated with the real ones by using 
   * directedPerceptionPTU::updatePanTilt(). Be aware that the values can only be updated, if
   * directedPerceptionPTU::isPTUidle() returns true. Otherwise the positions can not be read from
   * the unit.<br>
   *
   * <b>Determining an idle pan-tilt unit</b><br>
   * The method directedPerceptionPTU::isPTUidle() provides to check the PTU working
   * condition. Each time, when a method is called, which directly sends commands to the pan-tilt
   * unit, an instruction counter is increased. For every command which is successfully applied the 
   * unit sends an asterix '*' and for each failed command a '!'. The method reads out the serial buffer
   * and decreases the instruction counter with the number of '*' and '!' it has read. So, if all
   * commands have been worked out, the instruction counter is back to zero and the method returns
   * true. Otherwise, if the instruction counters value is greater than zero, it returns false.<br>
   * Note that in immidiate execution mode an asterix is already returned from the pan-tilt unit,
   * if the desired position can be driven, but has not started moving yet. That means, that 
   * directedPerceptionPTU::isPTUidle() returns also true, if the last position command is still
   * being executed. You can avoid this by subsequently calling directedPerceptionPTU::awaitPosCommandCompletion(). 
   * Then the idle method only returns true, when the pan-tilt unit has finished its move.<br>
   *
   * Here is a short example code for using the pan-tilt unit.<br>
   * \code
   * #include "ltiDirectedPerceptionPTU.h"
   * #include <iostream>
   * 
   * int main(void) {
   *   lti::directedPerceptionPTU ptu;
   *   ptu.initialize();			                                // Test the connection and make first settings
   *   ptu.setAngleFormat(lti::panTiltUnit::parameters::Degrees); // Set angle format to degrees
   *   ptu.setPan(90.f);                                          // Set Pan to 90
   *   ptu.awaitPosCommandCompletion();                           // Do not accept commands till ptu is in position  
   *   while ( !ptu.isPTuIdle() ) {                               // Wait until position is reached
   *     // do nothing
   *   }
   *   cout << "Internal position: " << ptu.getPan() << endl;
   *   cout << "Real     position: " << ptu.getCurrentPan() << endl;
   * 
   *   // Now this will cause a huge difference between real and internal position
   *   // Try to drive back to zero position in 5 degrees steps and relative positioning.
   *   ptu.setPosMode(lti::directedPerceptionPTU::parameters::relative);
   *   for ( int i=0; i<90; i=i+5 ) {
   *     ptu.setPan(-i);
   *   }
   *
   *   ptu.awaitPosCommandCompletion();         // Do not accept commands till ptu is in position  
   *   while ( !ptu.isPTuIdle() ) {             // Wait until position is reached
   *     // do nothing
   *   }
   *   // Print positions
   *   cout << "Internal position: " << ptu.getPan() << endl;
   *   cout << "Real     position: " << ptu.getCurrentPan() << endl;
   *   cout << "Difference       : " << ptu.getCurrentPan()-ptu.getPan() << " " << endl;
   *   ptu.updatePanTilt();                                  // Update internal parameters
   *   cout << "Updated parameters:" << endl;
   *   cout << "Internal position: " << ptu.getPan() << endl;
   *   cout << "Real     position: " << ptu.getCurrentPan() << endl;
   *   cout << "Difference       : " << ptu.getCurrentPan()-ptu.getPan() << " " << endl;
   *   // Drive to starting position
   *   ptu.setPosMode(lti::directedPerceptionPTU::parameters::relative);
   *   ptu.setPan(0.f);
   *   return(0);
   * }
   * \endcode
   * <br>
   */
  class directedPerceptionPTU : public panTiltUnit {
  public:
    /**
     * The parameters for the class DirectedPerceptionPTU
     */
    class parameters : public panTiltUnit::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      public:
      // Serial-Object
      serial connectionToPTU;
      
      // Response Time
      static const int ptuResponseTime;
      
      // Position Modes
      enum ePosModes { absolute, relative };
      ePosModes posMode;
      
      // Execution Modes
      enum eExecModes { immidiate , slaved };
      eExecModes execMode;
              
      // Speeds
      static const float maxPanSpeed;
      static const float maxTiltSpeed;
      static const float minPanSpeed;
      static const float minTiltSpeed;
      float panSpeed;
      float tiltSpeed;
      
      // Acceleration
      static const float maxPanAcceleration;
      static const float maxTiltAcceleration;
      static const float minPanAcceleration;
      static const float minTiltAcceleration;
      float panAcceleration;
      float tiltAcceleration;
      
      // Booleans 
      bool connectedToPTU;
            
      // Resolution
      static const float resolutionPerPosition;
      
      // Counters
      static int instructionCounter;
     
    };

    /**
     * Default constructor
     */
    directedPerceptionPTU();

    /**
     * Construct a functor using the given parameters
     */
    directedPerceptionPTU(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    directedPerceptionPTU(const directedPerceptionPTU& other);

    /**
     * Destructor
     */
    virtual ~directedPerceptionPTU();

    /**
     * Returns the name of this type ("DirectedPerceptionPTU")
     */
    virtual const char* getTypeName() const;

	/**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    directedPerceptionPTU& copy(const directedPerceptionPTU& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    directedPerceptionPTU& operator=(const directedPerceptionPTU& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Tests connection to pan-tilt unit and sets initial speeds, acceleration and position
     * @return true if a communication could be established. False, if serial port could not 
     * be accessed or timeout occured.
     */ 
    bool initialize();
    
    /**
     * Sets angle format to degrees or radiants.
     * @return false, if an error occured, otherwise true.
     * Standard angle format is in radiants.
     */
    bool setAngleFormat(parameters::eAngleFormatType anAngleFormat);
    /**
     * Sets command execution mode to immidiate or slaved.
     * @return false, if an error occured, otherwise true.
     * In immidiate mode pan-tilt unit is instructed to execute 
     * positional commands at once. In slaved mode position 
     * commands are only executed, when method directedPerceptionPTU::awaitPosCommandCompletion()
     * is applied.
     * Standard format is immidiate
     */
    bool setExecMode(parameters::eExecModes anExecMode);
    /**
     * Sets position mode to absolute or relative.
     * @return false, if an error occured, otherwise true.
     * In absolute mode the pan-tilt-unit drives directly to the
     * desired positions. In relative mode position commands are 
     * executed concerning the actual axis position. The offset is
     * then an angle distance between current and desired position. 
     * Standard position mode is absolute.
     */
    bool setPosMode(parameters::ePosModes anPosMode);
    
     /**
     * Reset the pan-tilt unit.
     * Forces the pan-tilt unit to drive through its pan and tilt axis to determine min/max positions.
     * After that pan-tilt position 0,0 is applied.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.
     */
    bool reset();
    
    /**
     * Stop all movement of the pan-tilt unit.
     * The pan-tilt unit freezes at its current pan and tilt position.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.
     */
    bool stopPanTilt();
    
    /**
     * Stop pan movement.
     * The pan-tilt unit freezes at its current pan position.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.
     */      
    bool stopPan();
    
    /*
     * Stop tilt movement.
     * The pan-tilt unit freezes at its current pan position.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true. 
     */
    bool stopTilt();
    
    /**
     * Set desired pan-tilt position.
     * If the given pan and tilt positions, which could be set in radiants or degrees, are within 
     * minimum/maximum range the pan-tilt unit will drive there. If the values are out of range, 
     * max or min positions are applied. This will be commented in the status string. 
     * Depending on posMode the position can be set in absolute or relative angle parameters.
     * Notise, that pan-tilt position commands in the slaved execution mode are only applied, when
     * the directedPerceptionPTU::awaitPosCommandCompletion() method is called after setting pan-tilt. If you give more than one
     * pan-tilt combination in slaved mode before calling await method, only the last will be executed.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.     
     */
    bool setPanTilt(float pan, float tilt);
    
     /**
     * Set desired pan position.
     * If the given pan position, which could be set in radiants or degrees, is within 
     * minimum/maximum range the pan-tilt unit will drive there. If the value is out of range, 
     * max or min position is applied. This will be commented in the status string. 
     * Depending on posMode the position can be set in absolute or relative angle parameters.
     * Notise, that pan-tilt position commands in the slaved execution mode are only applied, when
     * the directedPerceptionPTU::awaitPosCommandCompletion() method is called after setting pan-tilt. If you give more than one
     * pan-tilt combination in slaved mode before calling await method, only the last will be executed.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.     
     */
    bool setPan(float pan);
    
	/**
     * Set desired tilt position
     * If the given tilt position, which could be set in radiants or degrees, is within 
     * minimum/maximum range the pan-tilt unit will drive there. If the value is out of range, 
     * max or min position is applied. This will be commented in the status string. 
     * Depending on posMode the position can be set in absolute or relative angle parameters.
     * Notise, that pan-tilt position commands in the slaved execution mode are only applied, when
     * the directedPerceptionPTU::awaitPosCommandCompletion() method is called after setting pan-tilt. If you give more than one
     * pan-tilt combination in slaved mode before calling await method, only the last will be executed.
     * @return false if serial port could not be accessed, timeout occured or the pan-tilt unit was not 
     * initialized properly. Otherwise true.     
     */
	bool setTilt(float tilt);
    
	/**
	 * Get internal pan and tilt positions.
	 * Depending on the current angle format the values are stored in the given 
	 * reference parameters in radiants or degrees.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * @return true in every case
	 */
	bool getPanTilt(float& pan, float& tilt) const;
    
	/**
	 * Get internal pan position.
	 * Depending on the current angle format the value is returned in radiants or degrees. They 
	 * are always in absolute positions, independent from position mode.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * @return true in every case
	 */
	float getPan() const;

    /**
	 * Get internal tilt position.
	 * Depending on the current angle format the value is returned in radiants or degrees.
	 * It is always in absolute position, independent from position mode.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * @return true in every case
	 */
    float getTilt() const;
	
	/**
	 * Reads out physically occuring pan and tilt axis positions from the pan-tilt unit and 
	 * stores them in the given reference parameters. Depending on the current angle format 
	 * the values contain the absolute positions in degrees or radiants. They are always in 
	 * absolute positions, independent from position mode.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * This method uses the directedPerceptionPTU::isIdlePTU() to determine, if the unit is 
	 * still performing commands. If so, the positions can not be read out. 
	 * @return true if positions could be read correctly from the pan-tilt unit. False, if  
	 * read command could not be send, timeout occured, unproper initialization of the PTU or
	 * if the pan-tilt unit is not idle cause it's still peforming more than one position 
	 * command.
	 */
	bool getCurrentPanTilt(float& pan, float& tilt);
    
	/**
	 * Reads out physically occuring pan axis position from the pan-tilt unit and 
	 * store it in the given reference parameter. Depending on the current angle format 
	 * the value contains the absolute position in degrees or radiants. It is always in absolute 
	 * position, independent from position mode.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * This method uses the directedPerceptionPTU::isIdlePTU() to determine, if the unit is 
	 * still performig commands. If so, the positions can not be read out. 
	 * @return true if the position could be read correctly from the pan-tilt unit. False, if  
	 * read command could not be send, timeout occured, unproper initialization of the PTU or
	 * if the pan-tilt unit is not idle cause it's still peforming more than one position 
	 * command. This is reported inthe statusString.
	 */
	float getCurrentPan();
    
	/**
	 * Reads out physically occuring tilt axis position from the pan-tilt unit and 
	 * store it in the given reference parameter. Depending on the current angle format 
	 * the value contains the absolute position in degrees or radiants. It is always in absolute 
	 * position, independent from position mode.
	 * Note that the internal pan and tilt positions could differ very much from the physical
	 * ones, if you apply a large amount of relative position commands shortly after each other.
	 * You can update the internal values by using directedPerceptionPTU::updatePanTilt().
	 * This method uses the directedPerceptionPTU::isIdlePTU() to determine, if the unit is 
	 * still performig commands. If so, the positions can not be read out. 
	 * @return true if the position could be read correctly from the pan-tilt unit. False, if  
	 * read command could not be send, timeout occured, unproper initialization of the PTU or
	 * if the pan-tilt unit is not idle cause it's still peforming more than one position 
	 * command. This is reported in the status string.
	 */
	float getCurrentTilt();
    
	/**
	 * Enables autoscan in pan direction.
	 * In autoscan or monitor mode the pan-tilt unit keeps moving between the given 
	 * pan and/or tilt positions until another position command or
	 * directedPerceptionPTU::stopAutoscan() is applied. Through this method the unit
	 * is instructed to move between panPos1 and panPos2 with the current pan speed and 
	 * pan acceleration. Autoscan is only available in absolute positions. Please, make sure
	 * you stop autoscan with directedPerceptionPTU::stopAutoscan(). This is saver than by applying
	 * further position commands. If the given values are not within maximum and minimum pan range, 
	 * max/min positions are applied. 
	 * @return false, if serial port could not be accessed, timeout occured, unproper initialization
	 * of the PTU or the pan positions were given in relative values. This is reported in status 
	 * string. Otherwise the method returns true.  
	 */
    bool setAutoscan( float panPos1, float panPos2 );
    
	/**
	 * Enables autoscan in pan and tilt direction.
	 * In autoscan or monitor mode the pan-tilt unit keeps moving between the given 
	 * pan and/or tilt positions until another position command or
	 * directedPerceptionPTU::stopAutoscan() is applied. Through this method the unit
	 * is instructed to move between panPos1 and panPos2 and between tiltPos1 and tilt Pos2 with
	 * current pan/tilt speeds/accelerations. Autoscan is only available in absolute positions. 
	 * Please, make sure you stop autoscan with directedPerceptionPTU::stopAutoscan(). This is 
	 * saver than by applying further position commands. If the given values are not within
	 * maximum and minimum pan/tilt range, max/min positions are applied.
	 * @return false, if serial port could not be accessed, timeout occured, unproper initialization
	 * of the PTU or the pan/tilt positions were given in relative values. This is reported in 
	 * status string. Otherwise the method returns true.  
	 */
	bool setAutoscan( float panPos1, float panPos2, float tiltPos1, float tiltPos2 );
    
	/**
	 * Enables autoscan with the last applied autoscan parameters .
	 * In autoscan or monitor mode the pan-tilt unit keeps moving between the given 
	 * pan and/or tilt positions until another position command or
	 * directedPerceptionPTU::stopAutoscan() is applied. Through this method the unit
	 * is instructed to move between pan and tilt positions given, when executing autoscan 
	 * the last time. If it was not enabled before the pan-tilt unit starts moving between max
	 * and min pan positions in current pan speed and acceleration. Autoscan is only available 
	 * in absolute positions. Please, make sure you stop autoscan with directedPerceptionPTU::stopAutoscan(). 
	 * This is saver than by applying further position commands. If the given values are not within
	 * maximum and minimum pan/tilt range, max/min positions are applied.
	 * @return false, if serial port could not be accessed, timeout occured, unproper initialization
	 * of the PTU or the pan/tilt positions were given in relative values. This is reported in 
	 * status string. Otherwise the method returns true.  
	 */
	bool setAutoscan();
    
	/**
	 * Stops autoscan and forces the pan-tilt unit to drive to starting position ( Pan, Tilt ) 
	 * = ( 0, 0 ).
	 * @return false, if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
	bool stopAutoscan();
      
	/**
	 * Sets the pan speed to the desired value panSpeed, which can be given in radiants or degrees.
	 * It is always interpreted as absolute setting, independent from position mode. 
	 * If it is between allowed minimum and maximum speed, the change is dircetly taken over. 
	 * If the value is out of range max respectively min speed is applied. This case is indicated 
	 * by setting the status string.
	 * @return false , if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
    bool setPanSpeed ( float panSpeed  );

	/**
	 * Sets the tilt speed to the desired value panSpeed, which can be given in radiants or degrees.
	 * It is always interpreted as absolute setting, independent from position mode.
	 * If it is between allowed minimum and maximum speed, the change is dircetly taken over. 
	 * If the value is out of range max respectively min tilt speed is applied. This case is indicated 
	 * by setting the status string.
	 * @return false , if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
    bool setTiltSpeed( float tiltSpeed );
    
	/**
	 * Gets maximum pan speed in the desired angle format.
	 * @return the maximum pan speed in radiants or degrees per second
	 */
    float getMaxPanSpeed();

	/**
	 * Gets minimum pan speed in the desired angle format.
	 * @return the minimum pan speed in radiants or degrees per second
	 */
    float getMinPanSpeed();

	/**
	 * Gets maximum tilt speed in the desired angle format.
	 * @return the maximum tilt speed in radiants or degrees per second
	 */
    float getMaxTiltSpeed();

	/**
	 * Gets minimum tilt speed in the desired angle format.
	 * @return the minimum tilt speed in radiants or degrees per second
	 */
    float getMinTiltSpeed();
      
    /**
	 * Sets the pan acceleration to the desired value panAcc, which can be given in radiants or degrees.
	 * It is always interpreted as absolute setting, independent from position mode.
	 * If it is between allowed minimum and maximum pan acceleration, the change is dircetly taken over. 
	 * If the value is out of range max respectively min pan acceleration is applied. This case is indicated 
	 * by setting the status string.
	 * @return false , if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
    bool setPanAcceleration ( float panAcc  );

	/**
	 * Sets the tilt acceleration to the desired value panAcc, which can be given in radiants or degrees.
	 * It is always interpreted as absolute setting, independent from position mode.
	 * If it is between allowed minimum and maximum acceleration, the change is dircetly taken over. 
	 * If the value is out of range max respectively min tilt acceleration is applied. This case is indicated 
	 * by setting the status string.
	 * @return false , if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
    bool setTiltAcceleration( float tiltAcc );
    
    /**
	 * Gets maximum pan acceleration in the desired angle format.
	 * @return the maximum pan speed in radiants or degrees per second^2
	 */
    float getMaxPanAcceleration();

	/**
	 * Gets minimum pan acceleration in the desired angle format.
	 * @return the minimum pan acceleration speed in radiants or degrees per second^2
	 */
    float getMinPanAcceleration();

	/**
	 * Gets maximum tilt acceleration in the desired angle format.
	 * @return the maximum tilt acceleration speed in radiants or degrees per second^2
	 */
    float getMaxTiltAcceleration();

	/**
	 * Gets minimum tilt acceleration in the desired angle format.
	 * @return the minimum tilt acceleration speed in radiants or degrees per second^2
	 */
    float getMinTiltAcceleration();
    
    /**
	 * Determine whether the pan-tilt unit is still performing commands.
	 * That means, that at the moment at maximum one command is executed, which is not 
	 * a position command followed by an await instruction.<br>
	 * @return false, if serial port could not be accessed, timeout occured, the PTU was unproperly
	 * initialized, or the unit is not idle. This is reported in status string. Otherwise the method 
	 * returns true.  
	 */
    bool isPTUidle();
    
	/**
	 * Await the last position command to be completed.
	 * The pan-tilt unit is instructed to wait executing further commands until the last
	 * position command is completed. This means till the given pan and tilt positions are
	 * physically reached.
	 * @return false, if serial port could not be accessed, timeout occured or the PTU was unproperly
	 * initialized. This is reported in status string. Otherwise the method returns true.  
	 */
	bool awaitPosCommandCompletion();
    
	/**
	 * Reads out the current pan and tilt positions from the PTU and overrides the internal values.
	 * The method uses directedPTU::getCurrentPanTilt() to get the physically positions. <br>
	 * Note that the update can only be performed, if the pan-tilt unit is idle. That means, that
	 * at the moment at maximum one command is executed, which is not a position command followed 
	 * by an await instruction.<br>
	 * @return false, if serial port could not be accessed, timeout occured, the PTU was unproperly
	 * initialized, or the unit is not idle. This is reported in status string. Otherwise the method 
	 * returns true.  
	 */
	bool updatePanTilt();
    
    protected:
    /**
	 * @return a not-constant reference on a parameter object, which can be used to manipulate
	 * the parameter variable. This is necessary as the functor concept can not be strictly 
	 * kept in. Cause this would restrict the functionality of the class very much.
	 */
    parameters& getWritableParameters();
    
	/**
	 * Converts a given pan or tilt position into the internal PTU-format depending on the 
	 * position mode. 
	 * @return the given position in the internal format as integer.
	 */
	int convertValueToPTUnits(float position);
    
	/**
	 * Useful method, which simply reads out the serial buffer until it is empty to ensure,
	 * that nothing is in it before starting work on the buffer.
	 * @return nothing
	 */
	void emptySerialBuffer();
 
  };
}

#endif

