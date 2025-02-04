// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#define FUNCTION(f_, t_) { t_ = f_; }
#define MACRO(m_) { typeof(m_) v = m_; }
#define MACRO_VALUE(m_, v_) _Static_assert((m_)==(v_),#m_)
#define MACRO_TYPE(t_, m_) { t_ v = m_; }
#define TYPE(t_) { t_ value; }
#define INCOMPLETE_TYPE(t_) { t_* value; }
#define STRUCT_MEMBER(s_, t_, n_) { s_ s; t_* ptr = &(s.n_); }
#define STRUCT_MEMBER_ARRAY(s_, t_, n_) { s_ s; t_* ptr = &(s.n_[0]); }
#define STRUCT_MEMBER_FUNCTION_POINTER(s_, t_, n_) { s_ s; t_ = (s.n_); }
