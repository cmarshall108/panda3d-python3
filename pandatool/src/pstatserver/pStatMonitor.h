// Filename: pStatMonitor.h
// Created by:  drose (08Jul00)
// 
////////////////////////////////////////////////////////////////////

#ifndef PSTATMONITOR_H
#define PSTATMONITOR_H

#include <pandatoolbase.h>

#include "pStatClientData.h"
#include "pStatView.h"

#include <referenceCount.h>
#include <pointerTo.h>
#include <luse.h>

#include <map>

class PStatCollectorDef;

////////////////////////////////////////////////////////////////////
// 	 Class : PStatMonitor
// Description : This is an abstract class that presents the interface
//               to any number of different front-ends for the stats
//               monitor.  One of these will be created by the
//               PStatMonitor as each client is connected; this class
//               is responsible for opening up a new strip-chart graph
//               or whatever is appropriate.  It defines a number of
//               empty virtual functions that will be called as new
//               data becomes available.
////////////////////////////////////////////////////////////////////
class PStatMonitor : public ReferenceCount {
public:
  // The following functions are primarily for use by internal classes
  // to set up the monitor.
  PStatMonitor();
  virtual ~PStatMonitor();

  void hello_from(const string &hostname, const string &progname);
  void set_client_data(PStatClientData *client_data);


  // The following functions are for use by user code to determine
  // information about the client data available.
  bool is_alive() const;
  void close();

  INLINE const PStatClientData *get_client_data() const;
  INLINE string get_collector_name(int collector_index);
  const RGBColorf &get_collector_color(int collector_index);

  INLINE bool is_client_known() const;
  INLINE string get_client_hostname() const;
  INLINE string get_client_progname() const;

  PStatView &get_view(int thread_index);
  PStatView &get_level_view(int collector_index, int thread_index);


  // The following virtual methods may be overridden by a derived
  // monitor class to customize behavior.

  virtual string get_monitor_name()=0;

  virtual void initialized();
  virtual void got_hello();
  virtual void new_collector(int collector_index);
  virtual void new_thread(int thread_index);
  virtual void new_data(int thread_index, int frame_number);

  virtual void lost_connection();
  virtual void idle();
  virtual bool has_idle();

  virtual bool is_thread_safe();


private:
  PT(PStatClientData) _client_data;

  bool _client_known;
  string _client_hostname;
  string _client_progname;

  typedef map<int, PStatView> Views;
  Views _views;
  typedef map<int, Views> LevelViews;
  LevelViews _level_views;

  typedef map<int, RGBColorf> Colors;
  Colors _colors;
};

#include "pStatMonitor.I"

#endif
