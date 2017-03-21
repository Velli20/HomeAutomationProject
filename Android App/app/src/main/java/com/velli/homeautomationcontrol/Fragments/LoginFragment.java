/*
 *
 *  * MIT License
 *  *
 *  * Copyright (c) [2017] [velli20]
 *  *
 *  * Permission is hereby granted, free of charge, to any person obtaining a copy
 *  * of this software and associated documentation files (the "Software"), to deal
 *  * in the Software without restriction, including without limitation the rights
 *  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  * copies of the Software, and to permit persons to whom the Software is
 *  * furnished to do so, subject to the following conditions:
 *  *
 *  * The above copyright notice and this permission notice shall be included in all
 *  * copies or substantial portions of the Software.
 *  *
 *  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  * SOFTWARE.
 *
 */

package com.velli.homeautomationcontrol.Fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.TextInputLayout;
import android.support.v4.app.Fragment;
import android.support.v7.widget.AppCompatButton;
import android.support.v7.widget.AppCompatEditText;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.ProgressBar;

import com.velli.homeautomationcontrol.R;


public class LoginFragment extends Fragment implements View.OnClickListener {
    private AppCompatEditText mUsername;
    private AppCompatEditText mPassword;

    private TextInputLayout mUsernameLabel;
    private TextInputLayout mPasswordLabel;

    private AppCompatButton mLogin;
    private ProgressBar mLoginProgress;

    private OnDoLoginListener mListener;

    public interface OnDoLoginListener {
        void doLogin(String username, String password);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_login, container, false);

        mUsername = (AppCompatEditText) view.findViewById(R.id.login_field_username);
        mPassword = (AppCompatEditText) view.findViewById(R.id.login_field_password);

        mLogin = (AppCompatButton) view.findViewById(R.id.login_button);
        mLogin.setOnClickListener(this);

        mLoginProgress = (ProgressBar) view.findViewById(R.id.login_progress);

        mUsernameLabel = (TextInputLayout) view.findViewById(R.id.login_field_username_label);
        mUsernameLabel.setHint(getString(R.string.hint_username));
        mUsernameLabel.setEnabled(true);

        mPasswordLabel = (TextInputLayout) view.findViewById(R.id.login_field_password_label);
        mPasswordLabel.setHint(getString(R.string.hint_password));
        mPasswordLabel.setErrorEnabled(true);

        return view;
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();

        switch(id) {
            case R.id.login_button:
                doLogin();
                break;
        }
    }

    private void doLogin() {
        View view = getActivity().getCurrentFocus();
        if (view != null) {
            ((InputMethodManager) getActivity().getSystemService(Context.INPUT_METHOD_SERVICE)).
                    hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }

        String username = mUsername.getText().toString();
        String password = mPassword.getText().toString();

        if(username == null || username.isEmpty()) {
            mUsernameLabel.setError(getString(R.string.error_username_is_required));
            return;
        }
        if(password == null || password.isEmpty()) {
            mPasswordLabel.setError(getString(R.string.error_password_is_required));
            return;
        }

        if(mListener != null) {
            setLoggingIn(true);
            mListener.doLogin(username, password);
        }

    }

    public void setOnDoLoginListener(OnDoLoginListener l) {
        mListener = l;
    }

    public void setLoggingIn(boolean loggingIn) {
        mUsernameLabel.setError(null);
        mPasswordLabel.setError(null);

        mUsername.setEnabled(!loggingIn);
        mPassword.setEnabled(!loggingIn);
        mLogin.setEnabled(!loggingIn);

        mLogin.setVisibility(loggingIn ? View.INVISIBLE : View.VISIBLE);
        mLoginProgress.setVisibility(loggingIn ? View.VISIBLE : View.INVISIBLE);

    }

    public void setLoginError() {
        mPasswordLabel.setError(getString(R.string.error_wrong_password_or_username));
    }
}
