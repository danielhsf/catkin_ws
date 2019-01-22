;; Auto-generated. Do not edit!


(when (boundp 'programming_robots_with_ros::complex)
  (if (not (find-package "PROGRAMMING_ROBOTS_WITH_ROS"))
    (make-package "PROGRAMMING_ROBOTS_WITH_ROS"))
  (shadow 'complex (find-package "PROGRAMMING_ROBOTS_WITH_ROS")))
(unless (find-package "PROGRAMMING_ROBOTS_WITH_ROS::COMPLEX")
  (make-package "PROGRAMMING_ROBOTS_WITH_ROS::COMPLEX"))

(in-package "ROS")
;;//! \htmlinclude complex.msg.html


(defclass programming_robots_with_ros::complex
  :super ros::object
  :slots (_real _imaginary ))

(defmethod programming_robots_with_ros::complex
  (:init
   (&key
    ((:real __real) 0.0)
    ((:imaginary __imaginary) 0.0)
    )
   (send-super :init)
   (setq _real (float __real))
   (setq _imaginary (float __imaginary))
   self)
  (:real
   (&optional __real)
   (if __real (setq _real __real)) _real)
  (:imaginary
   (&optional __imaginary)
   (if __imaginary (setq _imaginary __imaginary)) _imaginary)
  (:serialization-length
   ()
   (+
    ;; float32 _real
    4
    ;; float32 _imaginary
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _real
       (sys::poke _real (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _imaginary
       (sys::poke _imaginary (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _real
     (setq _real (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _imaginary
     (setq _imaginary (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get programming_robots_with_ros::complex :md5sum-) "54da470dccf15d60bd273ab751e1c0a1")
(setf (get programming_robots_with_ros::complex :datatype-) "programming_robots_with_ros/complex")
(setf (get programming_robots_with_ros::complex :definition-)
      "float32 real
float32 imaginary
")



(provide :programming_robots_with_ros/complex "54da470dccf15d60bd273ab751e1c0a1")


