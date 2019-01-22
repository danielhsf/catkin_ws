; Auto-generated. Do not edit!


(cl:in-package programming_robots_with_ros-msg)


;//! \htmlinclude complex.msg.html

(cl:defclass <complex> (roslisp-msg-protocol:ros-message)
  ((real
    :reader real
    :initarg :real
    :type cl:float
    :initform 0.0)
   (imaginary
    :reader imaginary
    :initarg :imaginary
    :type cl:float
    :initform 0.0))
)

(cl:defclass complex (<complex>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <complex>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'complex)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name programming_robots_with_ros-msg:<complex> is deprecated: use programming_robots_with_ros-msg:complex instead.")))

(cl:ensure-generic-function 'real-val :lambda-list '(m))
(cl:defmethod real-val ((m <complex>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader programming_robots_with_ros-msg:real-val is deprecated.  Use programming_robots_with_ros-msg:real instead.")
  (real m))

(cl:ensure-generic-function 'imaginary-val :lambda-list '(m))
(cl:defmethod imaginary-val ((m <complex>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader programming_robots_with_ros-msg:imaginary-val is deprecated.  Use programming_robots_with_ros-msg:imaginary instead.")
  (imaginary m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <complex>) ostream)
  "Serializes a message object of type '<complex>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'real))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'imaginary))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <complex>) istream)
  "Deserializes a message object of type '<complex>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'real) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'imaginary) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<complex>)))
  "Returns string type for a message object of type '<complex>"
  "programming_robots_with_ros/complex")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'complex)))
  "Returns string type for a message object of type 'complex"
  "programming_robots_with_ros/complex")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<complex>)))
  "Returns md5sum for a message object of type '<complex>"
  "54da470dccf15d60bd273ab751e1c0a1")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'complex)))
  "Returns md5sum for a message object of type 'complex"
  "54da470dccf15d60bd273ab751e1c0a1")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<complex>)))
  "Returns full string definition for message of type '<complex>"
  (cl:format cl:nil "float32 real~%float32 imaginary~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'complex)))
  "Returns full string definition for message of type 'complex"
  (cl:format cl:nil "float32 real~%float32 imaginary~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <complex>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <complex>))
  "Converts a ROS message object to a list"
  (cl:list 'complex
    (cl:cons ':real (real msg))
    (cl:cons ':imaginary (imaginary msg))
))
