;; Auto-generated. Do not edit!


(when (boundp 'programming_robots_with_ros::WordCount)
  (if (not (find-package "PROGRAMMING_ROBOTS_WITH_ROS"))
    (make-package "PROGRAMMING_ROBOTS_WITH_ROS"))
  (shadow 'WordCount (find-package "PROGRAMMING_ROBOTS_WITH_ROS")))
(unless (find-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNT")
  (make-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNT"))
(unless (find-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNTREQUEST")
  (make-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNTREQUEST"))
(unless (find-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNTRESPONSE")
  (make-package "PROGRAMMING_ROBOTS_WITH_ROS::WORDCOUNTRESPONSE"))

(in-package "ROS")





(defclass programming_robots_with_ros::WordCountRequest
  :super ros::object
  :slots (_words ))

(defmethod programming_robots_with_ros::WordCountRequest
  (:init
   (&key
    ((:words __words) "")
    )
   (send-super :init)
   (setq _words (string __words))
   self)
  (:words
   (&optional __words)
   (if __words (setq _words __words)) _words)
  (:serialization-length
   ()
   (+
    ;; string _words
    4 (length _words)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _words
       (write-long (length _words) s) (princ _words s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _words
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _words (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass programming_robots_with_ros::WordCountResponse
  :super ros::object
  :slots (_count ))

(defmethod programming_robots_with_ros::WordCountResponse
  (:init
   (&key
    ((:count __count) 0)
    )
   (send-super :init)
   (setq _count (round __count))
   self)
  (:count
   (&optional __count)
   (if __count (setq _count __count)) _count)
  (:serialization-length
   ()
   (+
    ;; uint32 _count
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _count
       (write-long _count s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _count
     (setq _count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass programming_robots_with_ros::WordCount
  :super ros::object
  :slots ())

(setf (get programming_robots_with_ros::WordCount :md5sum-) "58903d21a3264f3408d79ba79e9f7c7e")
(setf (get programming_robots_with_ros::WordCount :datatype-) "programming_robots_with_ros/WordCount")
(setf (get programming_robots_with_ros::WordCount :request) programming_robots_with_ros::WordCountRequest)
(setf (get programming_robots_with_ros::WordCount :response) programming_robots_with_ros::WordCountResponse)

(defmethod programming_robots_with_ros::WordCountRequest
  (:response () (instance programming_robots_with_ros::WordCountResponse :init)))

(setf (get programming_robots_with_ros::WordCountRequest :md5sum-) "58903d21a3264f3408d79ba79e9f7c7e")
(setf (get programming_robots_with_ros::WordCountRequest :datatype-) "programming_robots_with_ros/WordCountRequest")
(setf (get programming_robots_with_ros::WordCountRequest :definition-)
      "string words
---
uint32 count

")

(setf (get programming_robots_with_ros::WordCountResponse :md5sum-) "58903d21a3264f3408d79ba79e9f7c7e")
(setf (get programming_robots_with_ros::WordCountResponse :datatype-) "programming_robots_with_ros/WordCountResponse")
(setf (get programming_robots_with_ros::WordCountResponse :definition-)
      "string words
---
uint32 count

")



(provide :programming_robots_with_ros/WordCount "58903d21a3264f3408d79ba79e9f7c7e")


