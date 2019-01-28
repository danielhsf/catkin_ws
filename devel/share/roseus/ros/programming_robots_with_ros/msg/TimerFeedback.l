;; Auto-generated. Do not edit!


(when (boundp 'programming_robots_with_ros::TimerFeedback)
  (if (not (find-package "PROGRAMMING_ROBOTS_WITH_ROS"))
    (make-package "PROGRAMMING_ROBOTS_WITH_ROS"))
  (shadow 'TimerFeedback (find-package "PROGRAMMING_ROBOTS_WITH_ROS")))
(unless (find-package "PROGRAMMING_ROBOTS_WITH_ROS::TIMERFEEDBACK")
  (make-package "PROGRAMMING_ROBOTS_WITH_ROS::TIMERFEEDBACK"))

(in-package "ROS")
;;//! \htmlinclude TimerFeedback.msg.html


(defclass programming_robots_with_ros::TimerFeedback
  :super ros::object
  :slots (_time_elapsed _time_remaining ))

(defmethod programming_robots_with_ros::TimerFeedback
  (:init
   (&key
    ((:time_elapsed __time_elapsed) (instance ros::time :init))
    ((:time_remaining __time_remaining) (instance ros::time :init))
    )
   (send-super :init)
   (setq _time_elapsed __time_elapsed)
   (setq _time_remaining __time_remaining)
   self)
  (:time_elapsed
   (&optional __time_elapsed)
   (if __time_elapsed (setq _time_elapsed __time_elapsed)) _time_elapsed)
  (:time_remaining
   (&optional __time_remaining)
   (if __time_remaining (setq _time_remaining __time_remaining)) _time_remaining)
  (:serialization-length
   ()
   (+
    ;; duration _time_elapsed
    8
    ;; duration _time_remaining
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; duration _time_elapsed
       (write-long (send _time_elapsed :sec) s) (write-long (send _time_elapsed :nsec) s)
     ;; duration _time_remaining
       (write-long (send _time_remaining :sec) s) (write-long (send _time_remaining :nsec) s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; duration _time_elapsed
     (send _time_elapsed :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _time_elapsed :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; duration _time_remaining
     (send _time_remaining :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _time_remaining :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get programming_robots_with_ros::TimerFeedback :md5sum-) "f7ef31d21e406bbd1f38a63801a29be7")
(setf (get programming_robots_with_ros::TimerFeedback :datatype-) "programming_robots_with_ros/TimerFeedback")
(setf (get programming_robots_with_ros::TimerFeedback :definition-)
      "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
#feedback
duration time_elapsed
duration time_remaining

")



(provide :programming_robots_with_ros/TimerFeedback "f7ef31d21e406bbd1f38a63801a29be7")

