--- a/slave.cpp
+++ b/slave.cpp
@@ -19,6 +19,24 @@

 using namespace lely;

+class MySlave : public canopen::BasicSlave {
+ public:
+  using BasicSlave::BasicSlave;
+
+ protected:
+  // This function gets called every time a value is written to the local object
+  // dictionary by an SDO or RPDO.
+  void
+  OnWrite(uint16_t idx, uint8_t subidx) noexcept override {
+    if (idx == 0x4000 && subidx == 0) {
+      // Read the value just written to object 4000:00, probably by RPDO 1.
+      uint32_t val = (*this)[0x4000][0];
+      // Copy it to object 4001:00, so that it will be sent by the next TPDO.
+      (*this)[0x4001][0] = val;
+    }
+  }
+};
+
 int
 main() {
   // Initialize the I/O library. This is required on Windows, but a no-op on
@@ -57,7 +75,7 @@
   chan.open(ctrl);

   // Create a CANopen slave with node-ID 2.
-  canopen::BasicSlave slave(timer, chan, "cpp-slave.eds", "", 2);
+  MySlave slave(timer, chan, "cpp-slave.eds", "", 2);

   // Create a signal handler.
   io::SignalSet sigset(poll, exec);
