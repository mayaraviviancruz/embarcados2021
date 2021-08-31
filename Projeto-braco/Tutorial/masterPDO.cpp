--- a/master.cpp
+++ b/master.cpp
@@ -67,6 +67,10 @@
       // Configure the heartbeat consumer on the master.
       ConfigHeartbeat(2000ms);

+      // Reset object 4000:00 and 4001:00 on the slave to 0.
+      Wait(AsyncWrite<uint32_t>(0x4000, 0, 0));
+      Wait(AsyncWrite<uint32_t>(0x4001, 0, 0));
+
       // Report success (empty error code).
       res({});
     } catch (canopen::SdoError& e) {
@@ -92,6 +96,23 @@
       res(e.code());
     }
   }
+
+  // This function gets called every time a value is written to the local object
+  // dictionary of the master by an RPDO (or SDO, but that is unlikely for a
+  // master), *and* the object has a known mapping to an object on the slave for
+  // which this class is the driver. The 'idx' and 'subidx' parameters are the
+  // object index and sub-index of the object on the slave, not the local object
+  // dictionary of the master.
+  void
+  OnRpdoWrite(uint16_t idx, uint8_t subidx) noexcept override {
+    if (idx == 0x4001 && subidx == 0) {
+      // Obtain the value sent by PDO from object 4001:00 on the slave.
+      uint32_t val = rpdo_mapped[0x4001][0];
+      // Increment the value and store it to an object in the local object
+      // dictionary that will be sent by TPDO to object 4000:00 on the slave.
+      tpdo_mapped[0x4000][0] = ++val;
+    }
+  }
 };

 int
