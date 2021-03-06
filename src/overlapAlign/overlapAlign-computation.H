
/******************************************************************************
 *
 *  This file is part of canu, a software program that assembles whole-genome
 *  sequencing reads into contigs.
 *
 *  This software is based on:
 *    'Celera Assembler' (http://wgs-assembler.sourceforge.net)
 *    the 'kmer package' (http://kmer.sourceforge.net)
 *  both originally distributed by Applera Corporation under the GNU General
 *  Public License, version 2.
 *
 *  Canu branched from Celera Assembler at its revision 4587.
 *  Canu branched from the kmer project at its revision 1994.
 *
 *  Modifications by:
 *
 *    Brian P. Walenz beginning on 2019-APR-22
 *      are a 'United States Government Work', and
 *      are released in the public domain
 *
 *  File 'README.licenses' in the root directory of this distribution contains
 *  full conditions and disclaimers for each license.
 */


//  This object is created by the loader thread, overlapReader().
//  During construction of this object, overlaps are loaded,
//  and a pointer to the cache is saved so we can retrieve the
//  reads on demand during the compute.

#include <tuple>

using namespace std;


class maComputation {
public:
  maComputation(uint32      id,
                trReadData *readData,
                sqCache    *seqCache,
                ovStore    *ovlStore,
                uint32      verboseTrim,
                uint32      verboseAlign) {

    _verboseTrim             = verboseTrim;
    _verboseAlign            = verboseAlign;

    _dovetailFraction        = 0.5;
    _coverage                = 20.0;

    _minLength               = 1000;
    _maxEdge                 = 2500;

    _overlapSlop             = 1000;
    _maxRepeatLength         = 2000;

    _wellContainedThreshold  = 1;

    _readData                = readData;

    _seqCache                = seqCache;

    //  Load overlaps.

    _overlapsMax = 0;
    _overlapsLen = ovlStore->loadOverlapsForRead(id, _overlaps, _overlapsMax);

    //  Allocate space for the a read.

    _aID   = id;
    _aMax  = _seqCache->sqCache_getLength(id) + 1;
    _aRead = new char [_aMax];

    //  Find the longest b read and allocate space for b read sequences.

    _bID   = UINT32_MAX;
    _bMax  = 0;

    for (uint32 ii=0; ii<_overlapsLen; ii++)
      _bMax = max(_bMax, _seqCache->sqCache_getLength(_overlaps[ii].b_iid) + 1);

    _bRead = new char [_bMax];

    //  Allocate space for the outputs.

    _alignsLen = 0;
    _alignsMax = _overlapsLen + 1;
    _alignsOvl = new ovOverlap * [_overlapsLen];
    _alignsA   = new char      * [_overlapsLen];
    _alignsB   = new char      * [_overlapsLen];
  };

  ~maComputation() {
    delete [] _overlaps;
    delete [] _aRead;
    delete [] _bRead;

    for (uint32 ii=0; ii<_alignsLen; ii++) {
      delete [] _alignsA[ii];
      delete [] _alignsB[ii];
    }

    delete [] _alignsOvl;
    delete [] _alignsA;
    delete [] _alignsB;
  };

private:
  bool                 isWellContained(void);
  void                 findMinThickestEdge(int32 &thick5, int32 &thick3);

  void                 fetchUntrimmedRead(uint32 id_, bool isA_=true, bool revComp_=false);
  void                 fetchTrimmedRead  (uint32 id_, bool isA_=true, bool revComp_=false);
  bool                 trimOverlap_Normal(ovOverlap *ovl);
  bool                 trimOverlap_Flipped(ovOverlap *ovl);
  bool                 trimOverlap(ovOverlap *ovl);

  void                 computeOverlapAlignment(ovOverlap   *ovl,
                                               uint32       minOverlapLength,
                                               double       maxErate,
                                               uint32       overlapSlop,
                                               uint32       maxRepeat);

public:
  void                 trimRead(uint32  minOverlapLength,
                                double  maxErate);

  void                 computeAlignments(uint32 minOverlapLength,
                                         double maxErate);

private:
  sqCache    *_seqCache;

public:
  uint32      _verboseTrim;
  uint32      _verboseAlign;

  //  Parameters, generally copied from the globals.

  double      _dovetailFraction;
  double      _coverage;

  int32       _minLength;
  int32       _maxEdge;

  uint32      _overlapSlop;
  uint32      _maxRepeatLength;

  uint32      _wellContainedThreshold;

  //  A pointer to the output object.

  trReadData *_readData;

  //  Overlaps are loaded by the loader thread during creation
  //  of this object.

  uint32      _overlapsMax;
  uint32      _overlapsLen;
  ovOverlap  *_overlaps;

  //  Sequence storage during alignment.  We assume the read
  //  sequences are cached in core, and that we can get them
  //  on demand, for free, during the computation.

  uint32      _aID,      _bID;
  uint32      _aMax,     _bMax;    //  Allocated length of aRead.
  char       *_aRead,   *_bRead;   //

  //  Alignment results.

  uint32      _alignsLen;
  uint32      _alignsMax;
  ovOverlap **_alignsOvl;
  char      **_alignsA;
  char      **_alignsB;
};
