/*
 *
 *  Copyright (C) 1998-2010, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module: dcmpstat
 *
 *  Author: Marco Eichelberg
 *
 *  Purpose:
 *    classes: DVPSStoredPrint_PList
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2010-10-14 13:16:36 $
 *  CVS/RCS Revision: $Revision: 1.11 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifndef DVPSSPL_H
#define DVPSSPL_H

#include "osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/dcmdata/dcvrus.h"
#include "dcmtk/dcmdata/dcvrui.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmpstat/dvpstyp.h"

class DVInterface;
class DVPSStoredPrint;
class DVConfiguration;
class DVPSPresentationLUT_PList;

/** a list of stored print objects, each of which manages a single Basic
 *  Film Box in a Print SCP.
 */
class DVPSStoredPrint_PList
{
public:
  /// default constructor
  DVPSStoredPrint_PList();

  /// copy constructor
  DVPSStoredPrint_PList(const DVPSStoredPrint_PList& copy);

  /** clone method.
   *  @return a pointer to a new DVPSStoredPrint_PList object containing
   *  a deep copy of this object.
   */
  DVPSStoredPrint_PList *clone() { return new DVPSStoredPrint_PList(*this); }

  /// destructor
  virtual ~DVPSStoredPrint_PList();

  /** reset the object to initial state.
   *  After this call, the object is in the same state as after
   *  creation with the default constructor.
   */
  void clear();

  /** get number of stored print objects in this list.
   *  @return the number of stored print objects.
   */
  size_t size() const { return list_.size(); }

  /** adds a Stored Print object to the list of managed objects. The stored
   *  print object becomes owned by this object and is destroyed upon
   *  destruction of the list.
   *  @param newSP Stored Print object to be added.
   */
  void insert(DVPSStoredPrint *newSP) { if (newSP) list_.push_back(newSP); }

  /** performs a Print SCP Basic Film Box N-SET operation.
   *  The results of the N-SET operation are stored in the
   *  objects passed as rsp and rspDataset.
   *  @param cfg config file facility
   *  @param cfgname symbolic printer name in config file
   *  @param rq N-SET request message
   *  @param rqDataset N-SET request dataset
   *  @param rsp N-SET response message
   *  @param rspDataset N-SET response dataset passed back in this parameter
   *  @param presentationLUTnegotiated
   *    OFTrue if support for the Presentation LUT SOP class
   *    has been negotiated at association negotiation
   *  @param globalPresentationLUTList
   *    list of presentation LUTs managed by the Print SCP
   */
  void printSCPBasicFilmBoxSet(
    DVConfiguration& cfg,
    const char *cfgname,
    T_DIMSE_Message& rq,
    DcmDataset *rqDataset,
    T_DIMSE_Message& rsp,
    DcmDataset *& rspDataset,
    OFBool presentationLUTnegotiated,
    DVPSPresentationLUT_PList& globalPresentationLUTList);

  /** performs a Print SCP Basic Grayscale Image Box N-SET operation.
   *  The results of the N-SET operation are stored in the
   *  objects passed as rsp and rspDataset.
   *  If successful, a Hardcopy Grayscale Image object containing
   *  the image data of the N-SET request is created in the database.
   *  @param cfg config file facility
   *  @param cfgname symbolic printer name in config file
   *  @param rq N-SET request message
   *  @param rqDataset N-SET request dataset
   *  @param rsp N-SET response message
   *  @param rspDataset N-SET response dataset passed back in this parameter
   *  @param presentationLUTnegotiated
   *    OFTrue if support for the Presentation LUT SOP class
   *    has been negotiated at association negotiation
   */
  void printSCPBasicGrayscaleImageBoxSet(
    DVInterface& cfg,
    const char *cfgname,
    T_DIMSE_Message& rq,
    DcmDataset *rqDataset,
    T_DIMSE_Message& rsp,
    DcmDataset *& rspDataset,
    OFBool presentationLUTnegotiated);

  /** performs a Print SCP Basic Film Box N-ACTION operation.
   *  The results of the N-ACTION operation are stored in the
   *  object passed as rsp.
   *  If successful, a Stored Print object containing the film box
   *  hierarchy is created in the database.
   *  @param cfg config file facility
   *  @param cfgname symbolic printer name in config file
   *  @param rq N-ACTION request message
   *  @param rsp N-ACTION response message
   *  @param globalPresentationLUTList list of presentation LUTs managed by the Print SCP
   */
  void printSCPBasicFilmBoxAction(
    DVInterface& cfg,
    const char *cfgname,
    T_DIMSE_Message& rq,
    T_DIMSE_Message& rsp,
    DVPSPresentationLUT_PList& globalPresentationLUTList);

  /** performs a Print SCP Basic Film Session N-ACTION operation.
   *  The results of the N-ACTION operation are stored in the
   *  object passed as rsp.
   *  If successful, one Stored Print object for each film box
   *  in the film session is created in the database.
   *  @param cfg config file facility
   *  @param cfgname symbolic printer name in config file
   *  @param rsp N-ACTION response message
   *  @param globalPresentationLUTList list of presentation LUTs managed by the Print SCP
   */
  void printSCPBasicFilmSessionAction(
    DVInterface& cfg,
    const char *cfgname,
    T_DIMSE_Message& rsp,
    DVPSPresentationLUT_PList& globalPresentationLUTList);

  /** performs a Print SCP basic film box N-DELETE operation.
   *  The results of the N-DELETE operation are stored in the object passed as rsp.
   *  @param rq N-DELETE request message
   *  @param rsp N-DELETE response message
   */
  void printSCPBasicFilmBoxDelete(T_DIMSE_Message& rq, T_DIMSE_Message& rsp);

  /** checks whether a film box object with the given SOP instance UID
   *  already exists.
   *  @param uid uid to be checked
   *  @return OFTrue if found, OFFalse otherwise
   */
  OFBool haveFilmBoxInstance(const char *uid);

  /** checks whether the Presentation LUT with the given UID
   *  is referenced by any Stored Print object in this list
   *  on the film box level.
   *  @param uid uid to be compared
   *  @return OFTrue if equal, OFFalse otherwise
   */
  OFBool usesPresentationLUT(const char *uid);

  /** checks whether the given Presentation LUT type could be used together
   *  with all image boxes in all film boxes on a Print SCP that requires a matching
   *  alignment between a Presentation LUT and the image pixel data.
   *  @param align LUT alignment type
   *  @return OFTrue if matching, OFFalse otherwise
   */
  OFBool matchesPresentationLUT(DVPSPrintPresentationLUTAlignment align) const;

  /** replaces the settings for illumination, reflected ambient light and
   *  referenced Presentation LUT in all film boxes in this list.
   *  Used by a Print SCP if Presentation LUT is implemented on Film Session
   *  level.
   *  @param newIllumination new value for illumination
   *  @param newReflectedAmbientLight new value for reflectedAmbientLight
   *  @param newReferencedPLUT new value for referenced presentation LUT instance UID
   *  @param newAlignment new alignment type of active presentation LUT
   */
  void overridePresentationLUTSettings(
      DcmUnsignedShort& newIllumination,
      DcmUnsignedShort& newReflectedAmbientLight,
      DcmUniqueIdentifier& newReferencedPLUT,
      DVPSPrintPresentationLUTAlignment newAlignment);

private:

  /// private undefined assignment operator
  DVPSStoredPrint_PList& operator=(const DVPSStoredPrint_PList&);

  /** the list maintained by this object
   */
  OFList<DVPSStoredPrint *> list_;
};

#endif

/*
 *  $Log: dvpsspl.h,v $
 *  Revision 1.11  2010-10-14 13:16:36  joergr
 *  Updated copyright header. Added reference to COPYRIGHT file.
 *
 *  Revision 1.10  2010-10-07 14:31:36  joergr
 *  Removed leading underscore characters from preprocessor symbols (reserved).
 *
 *  Revision 1.9  2010-03-01 09:08:49  uli
 *  Removed some unnecessary include directives in the headers.
 *
 *  Revision 1.8  2009-11-24 14:12:57  uli
 *  Switched to logging mechanism provided by the "new" oflog module.
 *
 *  Revision 1.7  2009-09-30 10:42:38  uli
 *  Make dcmpstat's include headers self-sufficient by including all
 *  needed headers directly and stop using dctk.h
 *
 *  Revision 1.6  2005-12-08 16:04:04  meichel
 *  Changed include path schema for all DCMTK header files
 *
 *  Revision 1.5  2003/06/04 10:18:06  meichel
 *  Replaced private inheritance from template with aggregation
 *
 *  Revision 1.4  2001/06/01 15:50:22  meichel
 *  Updated copyright header
 *
 *  Revision 1.3  2000/06/08 10:44:30  meichel
 *  Implemented Referenced Presentation LUT Sequence on Basic Film Session level.
 *    Empty film boxes (pages) are not written to file anymore.
 *
 *  Revision 1.2  2000/06/02 16:00:52  meichel
 *  Adapted all dcmpstat classes to use OFConsole for log and error output
 *
 *  Revision 1.1  2000/05/31 12:56:36  meichel
 *  Added initial Print SCP support
 *
 *
 */
